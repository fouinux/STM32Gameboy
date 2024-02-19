/*
 * ppu.c
 *
 *  Created on: 20 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include "ppu.h"
#include "mem.h"
#include "irq.h"
#include "debug.h"

#include <SDL2/SDL.h>

#define SCALE                       2

#define STATE_HBLANK_DURATION       51
#define STATE_VBLANK_DURATION       114
#define STATE_OAM_SEARCH_DURATION   20
#define STATE_PXL_XFER_DURATION     43

#define LINE_VISIBLE_MAX            144
#define LINE_MAX                    154

#define OAM_NB                      40

#define TILE_MAP_SIZE               32
#define TILE_SIZE                   16

#define get_tile_offset(ID)         (ID * TILE_SIZE)
#define get_tile_line_offset(ID, LINE)  (get_tile_offset(ID) + 2 * LINE)

struct tile_t
{
    struct
    {
        uint8_t Upper;
        uint8_t Lower;
    } Line[8];
};

struct oam_entry_t
{
    uint8_t Y;
    uint8_t X;
    uint8_t TileId;
    union
    {
        uint8_t Attributes;
        struct
        {
            uint8_t : 4;
            uint8_t Palette : 1;
            uint8_t X_Flip : 1;
            uint8_t Y_Flip : 1;
            uint8_t Priority : 1;
        } Attributes_Flags;
    };
};

struct ppu_t ppu;

/**
 * Search of 10 visible sprites
 */
static inline void exec_oam_search(void)
{
    struct oam_entry_t *pOam = (struct oam_entry_t*) ppu.pOAMTileMap;
    uint8_t sprite_size = ppu.pReg->LCDC_Flags.OBJSize ? 16 : 8;

    // Reset OAM counter
    ppu.OAM_counter = 0;

    for (int i = 0 ; i < OAM_NB ; i++)
    {
        // Search for Sprite on the line
        if (pOam[i].X != 0)
        {
            if (ppu.pReg->LY + 0x10 >= pOam[i].Y)
            {
                if (ppu.pReg->LY + 0x10 < pOam[i].Y + sprite_size)
                {
                    // Sprite is visible
                    ppu.aOAM_visible[ppu.OAM_counter++] = i;
                    if (ppu.OAM_counter >= PPU_OAM_VISIBLE_MAX)
                    {
                        // Stop search
                        break;
                    }
                }
            }
        }
    }

    // Sort coordinates
    if (ppu.OAM_counter > 1)
    {
        for (int round = 0 ; round < ppu.OAM_counter ; round++)
        {
            for (int i = 0 ; i < (ppu.OAM_counter - round - 1) ; i++)
            {
                if (pOam[ppu.aOAM_visible[i]].X > pOam[ppu.aOAM_visible[i+1]].X)
                {
                    uint8_t temp = ppu.aOAM_visible[i];
                    ppu.aOAM_visible[i] = ppu.aOAM_visible[i+1];
                    ppu.aOAM_visible[i+1] = temp;
                }

                // The last one is sorted, get it's position corrected by the offset
                ppu.aOAM_visible_x[ppu.OAM_counter - round - 1] = pOam[ppu.aOAM_visible[ppu.OAM_counter - round - 1]].X - 8;
            }
        }
    }

    // Set the first one coordinates
    if (ppu.OAM_counter > 0)
        ppu.aOAM_visible_x[0] = pOam[ppu.aOAM_visible[0]].X - 8;

    ppu.OAM_visible_id = 0;
}

// This table helps to convert raw tiles into 8 pixels by inverting bits order and add zero between each bits
static const uint16_t aTileConvertHelper[256] = {
    0x0000, 0x4000, 0x1000, 0x5000, 0x0400, 0x4400, 0x1400, 0x5400, 0x0100, 0x4100, 0x1100, 0x5100, 0x0500, 0x4500, 0x1500, 0x5500,
    0x0040, 0x4040, 0x1040, 0x5040, 0x0440, 0x4440, 0x1440, 0x5440, 0x0140, 0x4140, 0x1140, 0x5140, 0x0540, 0x4540, 0x1540, 0x5540,
    0x0010, 0x4010, 0x1010, 0x5010, 0x0410, 0x4410, 0x1410, 0x5410, 0x0110, 0x4110, 0x1110, 0x5110, 0x0510, 0x4510, 0x1510, 0x5510,
    0x0050, 0x4050, 0x1050, 0x5050, 0x0450, 0x4450, 0x1450, 0x5450, 0x0150, 0x4150, 0x1150, 0x5150, 0x0550, 0x4550, 0x1550, 0x5550,
    0x0004, 0x4004, 0x1004, 0x5004, 0x0404, 0x4404, 0x1404, 0x5404, 0x0104, 0x4104, 0x1104, 0x5104, 0x0504, 0x4504, 0x1504, 0x5504,
    0x0044, 0x4044, 0x1044, 0x5044, 0x0444, 0x4444, 0x1444, 0x5444, 0x0144, 0x4144, 0x1144, 0x5144, 0x0544, 0x4544, 0x1544, 0x5544,
    0x0014, 0x4014, 0x1014, 0x5014, 0x0414, 0x4414, 0x1414, 0x5414, 0x0114, 0x4114, 0x1114, 0x5114, 0x0514, 0x4514, 0x1514, 0x5514,
    0x0054, 0x4054, 0x1054, 0x5054, 0x0454, 0x4454, 0x1454, 0x5454, 0x0154, 0x4154, 0x1154, 0x5154, 0x0554, 0x4554, 0x1554, 0x5554,
    0x0001, 0x4001, 0x1001, 0x5001, 0x0401, 0x4401, 0x1401, 0x5401, 0x0101, 0x4101, 0x1101, 0x5101, 0x0501, 0x4501, 0x1501, 0x5501,
    0x0041, 0x4041, 0x1041, 0x5041, 0x0441, 0x4441, 0x1441, 0x5441, 0x0141, 0x4141, 0x1141, 0x5141, 0x0541, 0x4541, 0x1541, 0x5541,
    0x0011, 0x4011, 0x1011, 0x5011, 0x0411, 0x4411, 0x1411, 0x5411, 0x0111, 0x4111, 0x1111, 0x5111, 0x0511, 0x4511, 0x1511, 0x5511,
    0x0051, 0x4051, 0x1051, 0x5051, 0x0451, 0x4451, 0x1451, 0x5451, 0x0151, 0x4151, 0x1151, 0x5151, 0x0551, 0x4551, 0x1551, 0x5551,
    0x0005, 0x4005, 0x1005, 0x5005, 0x0405, 0x4405, 0x1405, 0x5405, 0x0105, 0x4105, 0x1105, 0x5105, 0x0505, 0x4505, 0x1505, 0x5505,
    0x0045, 0x4045, 0x1045, 0x5045, 0x0445, 0x4445, 0x1445, 0x5445, 0x0145, 0x4145, 0x1145, 0x5145, 0x0545, 0x4545, 0x1545, 0x5545,
    0x0015, 0x4015, 0x1015, 0x5015, 0x0415, 0x4415, 0x1415, 0x5415, 0x0115, 0x4115, 0x1115, 0x5115, 0x0515, 0x4515, 0x1515, 0x5515,
    0x0055, 0x4055, 0x1055, 0x5055, 0x0455, 0x4455, 0x1455, 0x5455, 0x0155, 0x4155, 0x1155, 0x5155, 0x0555, 0x4555, 0x1555, 0x5555
};

// This table helps to convert mirrored tiles into 8 pixels by inverting bits order and add zero between each bits
static const uint16_t aTileConvertMirrorHelper[256] = {
    0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015, 0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,
    0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115, 0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,
    0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415, 0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,
    0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515, 0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555,
    0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015, 0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055,
    0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115, 0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155,
    0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415, 0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455,
    0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515, 0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555,
    0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015, 0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055,
    0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115, 0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155,
    0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415, 0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455,
    0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515, 0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555,
    0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015, 0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055,
    0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115, 0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155,
    0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415, 0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455,
    0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515, 0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555
};

static inline void fetch_bg_tile(void)
{
    uint8_t tile_x = (ppu.pReg->SCX + ppu.x_fetch) >> 3;
    uint8_t tile_y = (ppu.pReg->SCY + ppu.pReg->LY) >> 3;
    uint16_t tile_map_id = tile_x + tile_y * TILE_MAP_SIZE;

    uint16_t tile_id = ppu.pBGTileMap[tile_map_id];
    struct tile_t *pTile = &((struct tile_t*) ppu.pBGWinTileData)[tile_id];

    uint8_t line = (ppu.pReg->SCY + ppu.pReg->LY) & 0x07;

    uint8_t upper = pTile->Line[line].Upper;
    uint8_t lower = pTile->Line[line].Lower;

    uint16_t result = aTileConvertHelper[upper] | (aTileConvertHelper[lower] << 1);

    // Extract and put pixels in FIFO
    for (uint8_t i = 0 ; i < 8 ; i++)
    {
        fifo_enqueue(&ppu.Fifo_BG, result & 0x03);
        result >>= 2;
    }
}

static inline void fetch_win_tile(void)
{
    uint8_t tile_x = (ppu.x_fetch - ppu.pReg->WX + 7) >> 3;
    uint8_t tile_y = (ppu.pReg->LY - ppu.pReg->WY) >> 3;
    uint16_t tile_map_id = tile_x + tile_y * TILE_MAP_SIZE;

    uint16_t tile_id = ppu.pWinTileMap[tile_map_id];
    struct tile_t *pTile = &((struct tile_t*) ppu.pBGWinTileData)[tile_id];

    uint8_t line = (ppu.pReg->SCY + ppu.pReg->LY) & 0x07;

    uint8_t upper = pTile->Line[line].Upper;
    uint8_t lower = pTile->Line[line].Lower;

    uint16_t result = aTileConvertHelper[upper] | (aTileConvertHelper[lower] << 1);

    // Extract and put pixels in FIFO
    for (uint8_t i = 0 ; i < 8 ; i++)
    {
        fifo_enqueue(&ppu.Fifo_BG, result & 0x03);
        result >>= 2;
    }
}

static inline void fetch_sprite(uint8_t id)
{
    struct oam_entry_t *pSprite = &((struct oam_entry_t*) ppu.pOAMTileMap)[id];
    struct tile_t *pTile = &((struct tile_t*)ppu.pOAMTileData)[pSprite->TileId];

    uint8_t line = ppu.pReg->LY - (pSprite->Y - 0x10);
    if (pSprite->Attributes_Flags.Y_Flip == 1)
    {
        if (ppu.pReg->LCDC_Flags.OBJSize == 0) // 8x8
            line = 8 - line;
        else // 8x16
            line = 16 - line;
    }

    // Use overflow to make it work perfectly for 8x8 and 8x16
    uint8_t upper = pTile->Line[line].Upper;
    uint8_t lower = pTile->Line[line].Lower;

    uint16_t result;
    if (pSprite->Attributes_Flags.X_Flip == 0)
        result = aTileConvertHelper[upper] | (aTileConvertHelper[lower] << 1);
    else
        result = aTileConvertMirrorHelper[upper] | (aTileConvertMirrorHelper[lower] << 1);

    // Extract and put pixels in FIFO
    for (uint8_t i = 0 ; i < 8 ; i++)
    {
        // Enqueue only if visible
        if (pSprite->X - 8 + i >= 0)
            fifo_enqueue(&ppu.Fifo_OAM, result & 0x03);
        result >>= 2;
    }
}

static inline void exec_pxl_xfer(void)
{
    // BG and Window disabled
    if (ppu.pReg->LCDC_Flags.BGEnable == 0)
    {
        for (int i = 0 ; i < 4 && ppu.x_draw < PPU_SCREEN_W  ; i++)
            ppu.aScreen[ppu.x_draw++][ppu.pReg->LY] = ppu.aColor[0]; // White
        return;
    }

    // Pixels mixing
    if (ppu.Fifo_BG.Size > 8)
    {
        for (int i = 0 ; i < 4 && ppu.x_draw < PPU_SCREEN_W  ; i++)
        {
            // Can out a pixel only if fifo has more than 8 pixels
            if (ppu.Fifo_BG.Size <= 8)
                break;

            // Sprite ?
            if (ppu.pReg->LCDC_Flags.OBJEnable == 1 && ppu.OAM_counter > 0)
            {
                if (ppu.aOAM_visible_x[ppu.OAM_visible_id] <= ppu.x_draw)
                {
                    // Fetch Sprite
                    fetch_sprite(ppu.aOAM_visible[ppu.OAM_visible_id]);
                    ppu.OAM_visible_id++;
                    ppu.OAM_counter--;

                    // TODO Multiple sprite at same x
                }
            }

            // Get BG pixel with palette
            uint8_t pxl = ppu.aBGP[fifo_dequeue(&ppu.Fifo_BG)];

            // Sprite pixel available
            if (ppu.Fifo_OAM.Size > 0)
            {
                uint8_t sprite_pxl = fifo_dequeue(&ppu.Fifo_OAM);
                if (sprite_pxl != 0) //  If not transparent, overwrite pixel
                    pxl = ppu.aOBP0[sprite_pxl];
            }

            // Discard SCX & 0x07 first elements
            if (ppu.SCX_lsb > 0)
            {
                ppu.SCX_lsb--;
                continue;
            }

            ppu.aScreen[ppu.x_draw++][ppu.pReg->LY] = ppu.aColor[pxl];

            // Start of window
            if (ppu.pReg->LCDC_Flags.WindowEnable && (ppu.pReg->LY >= ppu.pReg->WY))
            {
                if (ppu.x_draw + 7 == ppu.pReg->WX)
                {
                    fifo_flush(&ppu.Fifo_BG);
                    ppu.x_fetch = 0; // Reset to start drawing window
                    break;
                }
            }
        }
    }

    // Fetch BG or Window
    if (ppu.Fifo_BG.Size <= 8)
    {
        if (ppu.pReg->LCDC_Flags.WindowEnable && (ppu.pReg->LY >= ppu.pReg->WY))
        {
            if (ppu.x_draw + 7 >= ppu.pReg->WX)
                fetch_win_tile();
            else
                fetch_bg_tile();
        }
        else
            fetch_bg_tile();

        ppu.x_fetch += 8;
    }

}

static inline void ppu_print_reg(void)
{
    printf("PPU registers:\n");
    printf("\tLCDC = 0x%02X\tSTAT = 0x%02X\n", ppu.pReg->LCDC, ppu.pReg->STAT);
    printf("\tSCY = 0x%02X\tSCX = 0x%02X\n", ppu.pReg->SCY, ppu.pReg->SCX);
    printf("\tLY  = 0x%02X\tLYC = 0x%02X\n", ppu.pReg->LY, ppu.pReg->LYC);
    printf("\tBGP = 0x%02X\tOBP0 = 0x%02X\tOBP1 = 0x%02X\n", ppu.pReg->BGP.Value, ppu.pReg->OBP0.Value, ppu.pReg->OBP1.Value);
    printf("\tWY  = 0x%02X\tWX  = 0x%02X\n\n", ppu.pReg->WY, ppu.pReg->WX);
}

void ppu_init(void)
{
    ppu.pReg = (struct ppu_reg_t *) mem_get_register(PPU);

    ppu.state = STATE_OAM_SEARCH;
    ppu.state_counter = 0;

    // Start at y = 0 & x = 0
    ppu.pReg->LY = 0;
    ppu.x_draw = 0;
    ppu.x_fetch = 0;

    // Tile maps and data
    ppu_update_lcdc();
    ppu.pOAMTileMap = mem_get_oam_ram();
    ppu.pOAMTileData = mem_get_vram();

    // Palettes
    ppu_update_bgp();
    ppu_update_obp0();
    ppu_update_obp1();

    // Init FIFO
    fifo_init(&ppu.Fifo_BG);
    fifo_init(&ppu.Fifo_OAM);

    ppu.STAT_Irq = false;

    // SDL Specific
    SDL_PixelFormat *pPixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    ppu.aColor[0] = SDL_MapRGBA(pPixelFormat, 0xFF, 0xFF, 0xFF, 0xFF); // White
    ppu.aColor[1] = SDL_MapRGBA(pPixelFormat, 0xAA, 0xAA, 0xAA, 0xFF); // Light gray
    ppu.aColor[2] = SDL_MapRGBA(pPixelFormat, 0x55, 0x55, 0x55, 0xFF); // Dark gray
    ppu.aColor[3] = SDL_MapRGBA(pPixelFormat, 0x00, 0x00, 0x00, 0xFF); // Black

    // Create Display Window
    ppu.pWindow = SDL_CreateWindow("STM32 Gameboy",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               PPU_SCREEN_W * SCALE,
                               PPU_SCREEN_H * SCALE,
                               SDL_WINDOW_OPENGL);
    if(NULL == ppu.pWindow)
    {
        // In the case that the window could not be made...
        printf("Could not create display window: %s\n", SDL_GetError());
        return;
    }
    ppu.pRenderer = SDL_CreateRenderer(ppu.pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ppu.pTexture = SDL_CreateTexture(ppu.pRenderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                PPU_SCREEN_W,
                                PPU_SCREEN_H);
}

void ppu_destroy(void)
{
    // SDL Specific
    SDL_DestroyWindow(ppu.pWindow);
}

static inline void sdl_render_display(void)
{
    uint8_t *pPixels;
    int pitch;

    SDL_LockTexture(ppu.pTexture, NULL, (void**) &pPixels, &pitch);

    // Copy display buffer into SDL window
    for (int y = 0 ; y < PPU_SCREEN_H ; y++)
    {
        uint32_t *pRow = (uint32_t *)(pPixels + pitch*y);
        for (int x = 0 ; x < PPU_SCREEN_W ; x++)
        {
            pRow[x] = ppu.aScreen[x][y];
        }
    }

    SDL_UnlockTexture(ppu.pTexture);

    SDL_RenderClear(ppu.pRenderer);
    SDL_RenderCopy(ppu.pRenderer, ppu.pTexture, NULL, NULL);
    SDL_RenderPresent(ppu.pRenderer);
}

void ppu_exec(void)
{
    if (ppu.pReg->LCDC_Flags.DisplayEnable == 0)
        return;

    ppu.state_counter++;

    // Update Mode Flag
    ppu.pReg->STAT_Flags.ModeFlag = ppu.state;

    switch(ppu.state)
    {
        case STATE_HBLANK:
            if (ppu.state_counter >= (STATE_HBLANK_DURATION + STATE_PXL_XFER_DURATION))
            {
                ppu.pReg->LY++;
                ppu.state_counter = 0;
                if (ppu.pReg->LY >= LINE_VISIBLE_MAX)
                {
                    ppu.state = STATE_VBLANK;
                    irq.pIF->Flags.VBlank = 1; // VBlank Interrupt
                }
                else
                {
                    ppu.state = STATE_OAM_SEARCH;
                }
            }
            break;

        case STATE_VBLANK:
            if (ppu.state_counter >= STATE_VBLANK_DURATION)
            {
                ppu.pReg->LY++;
                if (ppu.pReg->LY >= LINE_MAX)
                {
                    sdl_render_display();
                    ppu.state_counter = 0;
                    ppu.pReg->LY = 0;
                    ppu.state = STATE_OAM_SEARCH;
                }
            }
            break;

        case STATE_OAM_SEARCH:
            // if (ppu.state_counter == 1 && ppu.pReg->LY == 0)
            //     ppu_print_reg();

            if (ppu.state_counter == 1)
                exec_oam_search();
            if (ppu.state_counter >= STATE_OAM_SEARCH_DURATION)
            {
                ppu.state_counter = 0;
                ppu.state = STATE_PXL_XFER;
                ppu.SCX_lsb = ppu.pReg->SCX & 0x07;
                ppu.x_draw = 0; // Reset at the begining of the scanline
                ppu.x_fetch = 0;

                // Flush pixesl FIFO
                fifo_flush(&ppu.Fifo_BG);
                fifo_flush(&ppu.Fifo_OAM);
            }
            break;

        case STATE_PXL_XFER:
            exec_pxl_xfer();
            if (ppu.x_draw >= PPU_SCREEN_W)
                ppu.state = STATE_HBLANK;
            break;
    }

    // LYC = LY
    ppu.pReg->STAT_Flags.LYCeqLY_Flag = (ppu.pReg->LY == ppu.pReg->LYC);

    // Handling PPU interrupts
    bool STAT_Irq = false;
    // LYC = LY
    if (ppu.pReg->STAT_Flags.LYCeqLY && ppu.pReg->STAT_Flags.LYCeqLY)
        STAT_Irq = true;
    // mode 0-2 interrupts
    if (ppu.pReg->STAT_Flags.Mode0_HBlank && ppu.state == STATE_HBLANK)
        STAT_Irq = true;
    if (ppu.pReg->STAT_Flags.Mode1_VBlank && ppu.state == STATE_VBLANK)
        STAT_Irq = true;
    if (ppu.pReg->STAT_Flags.Mode2_OAM && ppu.state == STATE_OAM_SEARCH)
        STAT_Irq = true;

    if (!ppu.STAT_Irq && STAT_Irq)
        irq.pIF->Flags.LCDC = 1;

    ppu.STAT_Irq = STAT_Irq;
}

void ppu_update_lcdc(void)
{
    uint8_t *pVRAM = mem_get_vram();
    ppu.pBGTileMap = &pVRAM[(ppu.pReg->LCDC_Flags.BGTileMapAddr == 0) ? 0x1800 : 0x1C00];
    ppu.pWinTileMap = &pVRAM[(ppu.pReg->LCDC_Flags.WindowTileMapAddr == 0) ? 0x1800 : 0x1C00];
    ppu.pBGWinTileData = &pVRAM[(ppu.pReg->LCDC_Flags.BGWindowTileData == 0) ? 0x0800 : 0x0000];
}

void ppu_update_bgp(void)
{
    ppu.aBGP[0] = ppu.pReg->BGP.ID0;
    ppu.aBGP[1] = ppu.pReg->BGP.ID1;
    ppu.aBGP[2] = ppu.pReg->BGP.ID2;
    ppu.aBGP[3] = ppu.pReg->BGP.ID3;
}

void ppu_update_obp0(void)
{
    ppu.aOBP0[0] = ppu.pReg->OBP0.ID0;
    ppu.aOBP0[1] = ppu.pReg->OBP0.ID1;
    ppu.aOBP0[2] = ppu.pReg->OBP0.ID2;
    ppu.aOBP0[3] = ppu.pReg->OBP0.ID3;
}

void ppu_update_obp1(void)
{
    ppu.aOBP1[0] = ppu.pReg->OBP1.ID0;
    ppu.aOBP1[1] = ppu.pReg->OBP1.ID1;
    ppu.aOBP1[2] = ppu.pReg->OBP1.ID2;
    ppu.aOBP1[3] = ppu.pReg->OBP1.ID3;
}

void ppu_print_bg(uint8_t *pPixels, int pitch)
{
    // Get BG map and data
    uint8_t* pVRAM = mem_get_vram();
    uint16_t BGTileMapOffset = (ppu.pReg->LCDC_Flags.BGTileMapAddr == 0) ? 0x1800 : 0x1C00;
    uint16_t BGTileDataOffset = (ppu.pReg->LCDC_Flags.BGWindowTileData == 0) ? 0x0800 : 0x0000;
    uint8_t* pBGTileMap = pVRAM + BGTileMapOffset;
    uint8_t* pBGTileData = pVRAM + BGTileDataOffset;

    // Tile map to render
    for (int y = 0 ; y < TILE_MAP_SIZE ; y++)
    {
        for (int x = 0 ; x < TILE_MAP_SIZE ; x++)
        {
            // Get BG tile id
            uint8_t tileId = *(pBGTileMap + x + y * TILE_MAP_SIZE);
            uint8_t *pTile = pBGTileData + get_tile_offset(tileId);

            // Draw the tile
            for (int i = 0 ; i < TILE_SIZE ; i+=2)
            {
                uint8_t upper = pTile[i];
                uint8_t lower = pTile[i+1];

                // Get the line
                uint32_t *p = (uint32_t *)(pPixels + pitch*(y*8+i/2));
                p += 8 * x; // correct the x

                for (int pixel = 0 ; pixel < 8 ; pixel++)
                {
                    // Print pixel
                    uint8_t colorId = (upper >> (7-pixel) & 0x01) + ((lower >> (7-pixel) & 0x01) << 1);
                    *p = ppu.aColor[ppu.aBGP[colorId]];
                    p++;
                }
            }
        }
    }
}