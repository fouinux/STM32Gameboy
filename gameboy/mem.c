/*
 * memory.c
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include "cpu.h"
#include "mem.h"
#include "ppu.h"
#include "joypad.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MEM_CARTRIDGE_ROM_BANK_MAX      128 // 128 * 16 kiB = 2MiB
#define MEM_CARTRIDGE_RAM_BANK_MAX      16  // 16 * 8 kiB = 128kiB

#define MEM_SRAM_SIZE                   8192 // 8 kiB
#define MEM_VRAM_SIZE                   8192 // 8 kiB
#define MEM_OAM_RAM_SIZE                160  // 40 Spites
#define MEM_HRAM_SIZE                   128
#define MEM_IO_PORTS_SIZE               128

#define MEM_DMA_SIZE                    160

struct memory_map_t
{
    bool BootROMEnabled;
    uint8_t *pBootROM; // BootROM

    // Array of pointers on cartridge ROM Banks
    uint8_t *aCartridgeROMBank[MEM_CARTRIDGE_ROM_BANK_MAX];

    // Mapped Banks
    uint8_t ROMIndex; // [0x4000 - 0x8000]
    uint8_t RAMIndex; // [0xA000 - 0xC000]

    // On board RAM
    uint8_t SRAM[MEM_SRAM_SIZE];
    uint8_t VRAM[MEM_VRAM_SIZE];
    uint8_t OAM_RAM[MEM_OAM_RAM_SIZE];
    uint8_t HRAM[MEM_HRAM_SIZE];
    uint8_t IOPorts[MEM_IO_PORTS_SIZE];

    bool dma_ongoing;

} mem;


// IO Ports map
static const bool aIOPortsMap[MEM_IO_PORTS_SIZE] =
{
  //   00     01     02     03     04     05     06     07     08     09     0A     0B     0C     0D     0E     0F
     true,  true,  true, false,  true,  true,  true,  true, false, false, false, false, false, false, false,  true, // 0xFF00
     true,  true,  true,  true,  true, false,  true,  true,  true,  true,  true,  true,  true,  true,  true, false, // 0xFF10
     true,  true,  true,  true,  true,  true,  true, false, false, false, false, false, false, false, false, false, // 0xFF20
     true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true, // 0xFF30
     true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true, false, false, false, false, // 0xFF40
     true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF50
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF60
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF70
};

// IO Ports action on Write
static const bool aIOPortsActionOnWrMap[MEM_IO_PORTS_SIZE] =
{
  //   00     01     02     03     04     05     06     07     08     09     0A     0B     0C     0D     0E     0F
     true, false, false, false,  true, false, false, false, false, false, false, false, false, false, false, false, // 0xFF00
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF10
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF20
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF30
     true, false, false, false, false, false,  true,  true,  true,  true, false, false, false, false, false, false, // 0xFF40
     true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF50
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF60
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, // 0xFF70
};

static void* mem_translation(uint16_t Addr, bool Override)
{
    switch (Addr & 0xF000)
    {
        case 0x0000: // ROM Bank #0
            if ((Addr < 0x100) && mem.BootROMEnabled)
                return &mem.pBootROM[Addr];
        case 0x1000:
        case 0x2000:
        case 0x3000:
            return &mem.aCartridgeROMBank[0][Addr];
        case 0x4000: // Mapped ROM Bank
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return &mem.aCartridgeROMBank[mem.ROMIndex][Addr - 0x4000];
        case 0x8000: // VRAM
        case 0x9000:
            if (Override || ppu.pReg->STAT_Flags.ModeFlag != STATE_PXL_XFER)
                return &mem.VRAM[Addr - 0x8000];
            return NULL; // Not accessible during Pixel Transfer
        case 0xA000:
        case 0xB000:
            return NULL; // TODO External RAM
        case 0xC000: // WRAM
        case 0xD000:
            return &mem.SRAM[Addr - 0xC000];
        case 0xE000: //  Echo of WRAM
            return &mem.SRAM[Addr - 0xE000];
        case 0xF000:
            switch (Addr & 0xFF00)
            {
                case 0xF000:
                case 0xF100:
                case 0xF200:
                case 0xF300:
                case 0xF400:
                case 0xF500:
                case 0xF600:
                case 0xF700:
                case 0xF800:
                case 0xF900:
                case 0xFA00:
                case 0xFB00:
                case 0xFC00:
                case 0xFD00:
                    return &mem.SRAM[Addr - 0xE000];
                case 0xFE00:
                    if (Addr < 0xFEA0) // OAM RAM
                    {
                        if (Override || ppu.pReg->STAT_Flags.ModeFlag < STATE_OAM_SEARCH)
                            return &mem.OAM_RAM[Addr - 0xFE00];
                        return NULL; // Not accessible during OAM Seach and Pixel Transfer
                    }
                    else
                        return NULL; // Empty
                case 0xFF00:
                    if (Addr < 0xFF80) // IO Ports
                    {
                        if (Override || aIOPortsMap[Addr -  0xFF00] == true)
                            return &mem.IOPorts[Addr - 0xFF00];
                        return NULL;
                    }
                    else
                        return &mem.HRAM[Addr - 0xFF80];
            }
    }

    return NULL;
}

static void start_dma(uint8_t Value)
{
    uint8_t *pSrc = mem_translation((uint16_t)Value << 8, true);
    memcpy(mem.OAM_RAM, pSrc, MEM_DMA_SIZE);
    mem.dma_ongoing = true;
}

static void action_on_w8(uint16_t Addr, uint8_t Value, uint8_t *pU8)
{
    switch (Addr)
    {
        case 0xFF00: // Joypad
            *pU8 = Value & 0x30;
            joypad_update();
            break;

        case 0xFF04: // Reset DIV
            *pU8 = 0;
            break;

        case 0xFF40: // LCDC
            *pU8 = Value;
            ppu_update_lcdc();
            break;

        case 0xFF46: // DMA
            *pU8 = Value;
            start_dma(Value);
            break;

        case 0xFF47: // BGP
            *pU8 = Value;
            ppu_update_bgp();
            break;

        case 0xFF48: // OBP0
            *pU8 = Value;
            ppu_update_obp0();
            break;

        case 0xFF49: // OBP1
            *pU8 = Value;
            ppu_update_obp1();
            break;

        case 0xFF50: // Disable boot ROM
            mem.BootROMEnabled = false;
            break;

    }
}

void mem_init()
{
    // Init BootROM location
    mem.pBootROM = (uint8_t *) 0x08100000;
    mem.BootROMEnabled = true;

    // Init Cartridge ROM banks location
    memset(mem.aCartridgeROMBank, 0, MEM_CARTRIDGE_ROM_BANK_MAX * sizeof(uint8_t *));
    mem.aCartridgeROMBank[0] = (uint8_t *) 0x08110000;
    mem.aCartridgeROMBank[1] = (uint8_t *) 0x08118000;

    // Map memory
    mem.ROMIndex = 1;
    mem.RAMIndex = 0;
}

uint8_t mem_read_u8(uint16_t Addr)
{
    uint8_t *pU8 = (uint8_t *) mem_translation(Addr, false);
    if (NULL == pU8)
        return 0xFF;
    else
        return *pU8;
}

int8_t mem_read_s8(uint16_t Addr)
{
    int8_t *pS8 = (int8_t *) mem_translation(Addr, false);
    if (NULL == pS8)
        return 0xFF;
    else
        return *pS8;
}

uint16_t mem_read_u16(uint16_t Addr)
{
    uint16_t *pU16 = (uint16_t *) mem_translation(Addr, false);
    if (NULL == pU16)
        return 0xFFFF;
    else
        return *pU16;
}

void mem_write_u8(uint16_t Addr, uint8_t Value)
{
    uint8_t *pU8 = (uint8_t *) mem_translation(Addr, false);
    if (NULL != pU8)
    {
        if (Addr >= 0xFF00 && Addr <= 0xFF7F && aIOPortsActionOnWrMap[Addr -  0xFF00] == true)
            action_on_w8(Addr, Value, pU8);
        else
            *pU8 = Value;
    }

    if (Addr == 0xFFE1) // GAME_STATUS
    {
        printf("GS = %02x (from %04X)\n", Value, cpu.reg.PC);
    }
}

void mem_write_u16(uint16_t Addr, uint16_t Value)
{
    uint16_t *pU16 = (uint16_t *) mem_translation(Addr, false);
    if (NULL != pU16)
        *pU16 = Value;
}

uint8_t* mem_get_register(enum IOPorts_reg reg)
{
    switch (reg)
    {
        case JOYPAD:
            return &mem.IOPorts[0x00];
        case SERIAL:
            return &mem.IOPorts[0x01];
        case TIMER:
            return &mem.IOPorts[0x04];
        case SOUND:
            return &mem.IOPorts[0x10];
        case PPU:
            return &mem.IOPorts[0x40];
        case IF:
            return &mem.IOPorts[0x0F];
        case IE:
            return &mem.HRAM[0x7F];
        case BOOT:
            return &mem.IOPorts[0x50];
        default:
            return NULL;
    }
}

uint8_t* mem_get_oam_ram(void)
{
    return &mem.OAM_RAM[0];
}

uint8_t* mem_get_vram(void)
{
    return &mem.VRAM[0];
}

void mem_set_bootrom(uint8_t *pBootROM)
{
    mem.pBootROM = pBootROM;
}

void mem_set_gamerom(uint8_t *pGameROM, uint8_t index)
{
    mem.aCartridgeROMBank[index] = pGameROM;
}

void mem_hexdump(const uint16_t Addr, const size_t Size)
{
    printf("Offset(h) | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    printf("-----------------------------------------------------------\n");
    for (int a = Addr ; a < Addr + Size ; a += 16)
    {
        uint8_t *pMem = (uint8_t *)mem_translation(a, true);
        printf("%08X  | ", a);
        for (int i = 0 ; i < 16 ; i++)
        {
            printf("%02X ", pMem[i]);
        }
        printf("\n");
    }
}
