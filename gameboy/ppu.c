/*
 * ppu.c
 *
 *  Created on: 20 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include "ppu.h"
#include "mem.h"

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
    struct oam_entry_t *pOam = (struct oam_entry_t *) mem_get_oam_ram();
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

    // TODO Improve sort ?
    for (int round = 0 ; round < PPU_OAM_VISIBLE_MAX ; round++)
    {
        for (int i = 0 ; i < (PPU_OAM_VISIBLE_MAX - round - 1) ; i++)
        {
            if (ppu.aOAM_visible[i] > ppu.aOAM_visible[i+1])
            {
                uint8_t temp = ppu.aOAM_visible[i];
                ppu.aOAM_visible[i] = ppu.aOAM_visible[i+1];
                ppu.aOAM_visible[i+1] = temp;
            }
        }
    }
}

static inline void exec_pxl_xfer(void)
{
    // BG and Window disabled
    if (ppu.pReg->LCDC_Flags.BGEnable == 0)
    {
        ppu.aScreen[ppu.x++][ppu.pReg->LY] = ppu.aColor[0]; // White
        ppu.aScreen[ppu.x++][ppu.pReg->LY] = ppu.aColor[0]; // White
        ppu.aScreen[ppu.x++][ppu.pReg->LY] = ppu.aColor[0]; // White
        ppu.aScreen[ppu.x++][ppu.pReg->LY] = ppu.aColor[0]; // White
        return;
    }

    // FIFO mixer
    if (ppu.Fifo_BG.Size > 8)
    {
        // Mapping BG colors
        uint8_t aBGPalette[4];
        aBGPalette[0] = ppu.pReg->BGP >> 0 & 0x03;
        aBGPalette[1] = ppu.pReg->BGP >> 2 & 0x03;
        aBGPalette[2] = ppu.pReg->BGP >> 4 & 0x03;
        aBGPalette[3] = ppu.pReg->BGP >> 6 & 0x03;

        for (int i = 0 ; i < 4 ; i++)
        {
            // Can out a pixel only if fifo has more than 8 pixels
            if (ppu.Fifo_BG.Size <= 8)
                break;

            uint8_t bg = fifo_dequeue(&ppu.Fifo_BG);

            // TODO Sprite

            // Discard SCX & 0x07 first elements
            if (ppu.SCX_lsb > 0)
            {
                ppu.SCX_lsb--;
                continue;
            }

            // printf("PPU: Display pixels: %d @ %d\n", bg, ppu.x);
            ppu.aScreen[ppu.x++][ppu.pReg->LY] = aBGPalette[bg];
        }
    }

    uint8_t* pVRAM = mem_get_vram();

    // Fetch BG
    if (ppu.pReg->LCDC_Flags.WindowEnable == 0 && ppu.Fifo_BG.Size <= 8)
    {
        // Get BG map and data
        uint16_t BGTileMapOffset = (ppu.pReg->LCDC_Flags.BGTileMapAddr == 0) ? 0x1800 : 0x1C00;
        uint16_t BGTileDataOffset = (ppu.pReg->LCDC_Flags.BGWindowTileData == 0) ? 0x0800 : 0x0000;
        uint8_t* pBGTileMap = pVRAM + BGTileMapOffset;
        uint8_t* pBGTileData = pVRAM + BGTileDataOffset;

        uint8_t tileXId = (ppu.pReg->SCX + ppu.x) >> 3;
        uint8_t tileYId = (ppu.pReg->SCY + ppu.pReg->LY) >> 3;
        uint8_t tileId = *(pBGTileMap + tileXId + (tileYId * TILE_MAP_SIZE));
        uint8_t tileLine = (ppu.pReg->SCY + ppu.pReg->LY) & 0x07;

        // Offset to the correct line
        uint8_t *pTile = pBGTileData + get_tile_line_offset(tileId, tileLine);

        uint8_t upper = pTile[0];
        uint8_t lower = pTile[1];

        // Extract and put pixels in FIFO
        for (uint8_t i = 0 ; i < 8 ; i++)
        {
            uint8_t colorId = (upper >> 7) | ((lower >> 6) & 0x02);
            fifo_enqueue(&ppu.Fifo_BG, colorId);
            upper <<= 1;
            lower <<= 1;
        }

    }

    //  Fetch OBJ
    if (ppu.pReg->LCDC_Flags.OBJEnable)
    {

    }


}

void ppu_init(void)
{
    ppu.pReg = (struct ppu_reg_t *) mem_get_register(PPU);

    ppu.state = STATE_OAM_SEARCH;
    ppu.state_counter = 0;

    // Start at y = 0 & x = 0
    ppu.pReg->LY = 0;
    ppu.x = 0;

    // Init FIFO
    fifo_init(&ppu.Fifo_BG);
    fifo_init(&ppu.Fifo_OAM);

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
    ppu.pRenderer = SDL_CreateRenderer(ppu.pWindow, -1, 0);
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
        uint32_t *p = (uint32_t *)(pPixels + pitch*y);
        for (int x = 0 ; x < PPU_SCREEN_W ; x++)
        {
            *p = ppu.aColor[ppu.aScreen[x][y]];
            p++;
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

    // enum ppu_state_t currentState = ppu.state;

    ppu.state_counter++;

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
                    sdl_render_display();
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
                    ppu.state_counter = 0;
                    ppu.pReg->LY = 0;
                    ppu.pReg->LY = 0;
                    ppu.state = STATE_OAM_SEARCH;
                }
            }
            break;

        case STATE_OAM_SEARCH:
            if (ppu.state_counter == 0)
                exec_oam_search();
            if (ppu.state_counter >= STATE_OAM_SEARCH_DURATION)
            {
                ppu.state_counter = 0;
                ppu.state = STATE_PXL_XFER;
                ppu.SCX_lsb = ppu.pReg->SCX & 0x07;
                ppu.x = 0; // Reset at the begining of the scanline
            }
            break;

        case STATE_PXL_XFER:
            exec_pxl_xfer();
            if (ppu.x >= PPU_SCREEN_W)
            {
                ppu.state = STATE_HBLANK;
            }
            break;
    }

    // if (currentState != ppu.state)
    //     printf("PPU State %d => %d\n", currentState, ppu.state);
}

void ppu_print_bg(uint8_t *pPixels, int pitch)
{
    // Mapping BG colors
    uint8_t aBGPalette[4];
    aBGPalette[0] = ppu.pReg->BGP >> 0 & 0x03;
    aBGPalette[1] = ppu.pReg->BGP >> 2 & 0x03;
    aBGPalette[2] = ppu.pReg->BGP >> 4 & 0x03;
    aBGPalette[3] = ppu.pReg->BGP >> 6 & 0x03;

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
                    *p = ppu.aColor[aBGPalette[colorId]];
                    p++;
                }
            }
        }
    }
}