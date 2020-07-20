/*
 * ppu.c
 *
 *  Created on: 20 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/ppu.h>

struct ppu_reg_t
{
    union
    {
        uint8_t LCDC; // LCD Control
        struct
        {
            uint8_t DisplayEnable : 1;
            uint8_t WindowTileMapAddr : 1;
            uint8_t WindowEnable : 1;
            uint8_t BGWindowTileData : 1;
            uint8_t BGTileMapAddr : 1;
            uint8_t OBJSize : 1;
            uint8_t OBJEnable : 1;
            uint8_t BGEnable : 1;
        } LCDC_Flags;
    };

    union
    {
        uint8_t STAT; // LCD Status
        struct
        {
            uint8_t : 1;
            uint8_t LYCeqLY : 1;
            uint8_t Mode2_OAM : 1;
            uint8_t Mode1_VBlank : 1;
            uint8_t Mode0_HBlank : 1;
            uint8_t LYCeqLY_Flag : 1;
            uint8_t ModeFlag : 2;
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
