/*
 * ppu.h
 *
 *  Created on: 21 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_PPU_H_
#define INC_PPU_H_

#include <stdint.h>
#include "fifo.h"

#include <stdbool.h>
#include <SDL2/SDL.h>

#define PPU_SCREEN_W    160
#define PPU_SCREEN_H    144

#define PPU_BG_W        256
#define PPU_BG_H        256

#define PPU_OAM_VISIBLE_MAX 10

enum ppu_state_t
{
    STATE_HBLANK = 0,
    STATE_VBLANK,
    STATE_OAM_SEARCH,
    STATE_PXL_XFER,
};

struct ppu_reg_t
{
    union
    {
        uint8_t LCDC; // LCD Control
        struct
        {
            uint8_t BGEnable : 1;
            uint8_t OBJEnable : 1;
            uint8_t OBJSize : 1;
            uint8_t BGTileMapAddr : 1;
            uint8_t BGWindowTileData : 1;
            uint8_t WindowEnable : 1;
            uint8_t WindowTileMapAddr : 1;
            uint8_t DisplayEnable : 1;
        } LCDC_Flags;
    };

    union
    {
        uint8_t STAT; // LCD Status
        struct
        {
            uint8_t ModeFlag : 2;
            uint8_t LYCeqLY_Flag : 1;
            uint8_t Mode0_HBlank : 1;
            uint8_t Mode1_VBlank : 1;
            uint8_t Mode2_OAM : 1;
            uint8_t LYCeqLY : 1; // LYC = LY interrupt
            uint8_t : 1;
        } STAT_Flags;
    };

    uint8_t SCY;    // Scroll Y
    uint8_t SCX;    // Scroll X
    uint8_t LY;     // LCDC Y-Coordinate
    uint8_t LYC;    // LY Compare
    uint8_t DMA;    // DMA Transfer and Start
    uint8_t BGP;    // BG Palette
    uint8_t OBP0;   // Object Palette 0
    uint8_t OBP1;   // Object Palette 1
    uint8_t WY;     // Window Y Position
    uint8_t WX;     // Window X Position
} __attribute__ ((__packed__));

struct ppu_t
{
    struct ppu_reg_t *pReg;
    enum ppu_state_t state;
    uint8_t state_counter;
    uint8_t xfer_len;

    uint8_t OAM_counter;
    uint8_t aOAM_visible[PPU_OAM_VISIBLE_MAX];
    int16_t aOAM_visible_x[PPU_OAM_VISIBLE_MAX];
    uint8_t OAM_visible_id;

    // Tile maps and data
    uint8_t *pBGTileMap;
    uint8_t *pWinTileMap;
    uint8_t *pBGWinTileData;
    uint8_t *pOAMTileMap;
    uint8_t *pOAMTileData;

    // Palettes
    uint8_t aBGP[4];
    uint8_t aOBP0[4];
    uint8_t aOBP1[4];

    // Screen rendering
    uint8_t y_win_internal;
    struct fifo_t Fifo_BG;
    struct fifo_t Fifo_OAM;

    // Interrupts
    bool STAT_Irq;

    // Hardware dependent rendering
    uint32_t *pColor;
    uint8_t *pPixels;
    int pitch;
    uint32_t *pScreenRow;
};

extern struct ppu_t ppu;

void ppu_init(void);
void ppu_set_color(uint32_t *pColor);
void ppu_set_video_buffer(uint8_t* pPixels, int pitch);
bool ppu_exec(void);

void ppu_update_lcdc(void);
void ppu_update_bgp(void);
void ppu_update_obp0(void);
void ppu_update_obp1(void);

#ifdef PPU_DEBUG
void ppu_print_tiles(uint8_t *pPixels, int pitch);
void ppu_print_bg(uint8_t *pPixels, int pitch);
#endif

#endif /* INC_PPU_H_ */
