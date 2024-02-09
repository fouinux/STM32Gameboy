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
};

struct ppu_t
{
    struct ppu_reg_t *pReg;
    enum ppu_state_t state;
    uint8_t state_counter;

    uint8_t OAM_counter;
    uint8_t aOAM_visible[PPU_OAM_VISIBLE_MAX];
    uint8_t aOAM_visible_x[PPU_OAM_VISIBLE_MAX];
    uint8_t OAM_visible_id;

    // Screen rendering
    uint8_t x_draw;
    uint8_t x_fetch;
    uint8_t aScreen[PPU_SCREEN_W][PPU_SCREEN_H];
    struct fifo_t Fifo_BG;
    struct fifo_t Fifo_OAM;

    // Current scanline related
    uint8_t SCX_lsb; // Save SCX & 0x07 value a the begining of the scanline

    bool STAT_Irq;

    // SDL Specific
    SDL_Texture* pTexture;
    SDL_Window* pWindow;
    SDL_Renderer *pRenderer;
    uint32_t aColor[4];
};

extern struct ppu_t ppu;

void ppu_init(void);
void ppu_destroy(void);
void ppu_exec(void);

void ppu_print_bg(uint8_t *pPixels, int pitch);

#endif /* INC_PPU_H_ */
