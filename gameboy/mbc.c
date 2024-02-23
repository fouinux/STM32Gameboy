/*
 * mbc.c
 *
 *  Created on: 24 feb. 2024
 *      Author: Guillaume Fouilleul
 */

#include "mbc.h"
#include "mem.h"

#include <stdio.h>
#include <stdbool.h>

struct mbc1_t
{
    bool RAM_Enabled;
    uint8_t ROMIndex_L;
    uint8_t ROMIndex_H;
    bool BankingMode; // 0 : simple; 1 : advanced
};

struct mbc_t
{
    struct mbc1_t MBC1;
} mbc;

void mbc1(uint16_t Addr, uint8_t Value)
{
    // Decode MBC request
    switch (Addr & 0x6000) // Look only bits 13 & 14
    {
        case 0x0000: // RAM Enable
            mbc.MBC1.RAM_Enabled = ((Value & 0x0F) == 0x0A);
            break;
        case 0x2000: // ROM Bank Number
            mbc.MBC1.ROMIndex_L = Value & 0x1F;
            break;
        case 0x4000: // RAM Bank Number
            mbc.MBC1.ROMIndex_H = Value & 0x03;
            break;
        case 0x6000: // Banking Mode Select
            mbc.MBC1.BankingMode = Value & 0x01;
            break;
    }

    // Update memory
    if (mbc.MBC1.BankingMode == 0)
    {
        mem_set_rombank0(0);
        mem_set_rambank(mbc.MBC1.RAM_Enabled, 0);
    }
    else
    {
        mem_set_rombank0(mbc.MBC1.ROMIndex_H << 5);
        mem_set_rambank(mbc.MBC1.RAM_Enabled, mbc.MBC1.ROMIndex_H);
    }

    mem_set_rombank1((mbc.MBC1.ROMIndex_L == 0) ? 1 : mbc.MBC1.ROMIndex_L);
}

void mbc_unknown(uint16_t Addr, uint8_t Value)
{
    printf("MBC: Unknown action\n");
}

mbc_func_t mbc_get_callback(uint8_t Code)
{
    switch (Code)
    {
        case 0x00: // ROM Only
            return NULL;
        case 0x01: // MBC1
        case 0x02: // MBC1 + RAM
        case 0x03: // MBC1 + RAM + Battery
            return &mbc1;
        default: //  Not supported
            return &mbc_unknown;
    }
}
