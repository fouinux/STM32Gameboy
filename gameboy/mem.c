/*
 * mem.c
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include "cpu.h"
#include "mem.h"
#include "mbc.h"
#include "ppu.h"
#include "joypad.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MEM_SRAM_SIZE                   8192 // 8 kiB
#define MEM_VRAM_SIZE                   8192 // 8 kiB
#define MEM_OAM_RAM_SIZE                160  // 40 Spites
#define MEM_HRAM_SIZE                   128
#define MEM_IO_PORTS_SIZE               128

#define BOOT_ROM_SIZE                   256

#define MEM_DMA_SIZE                    160

struct cartridge_header_t
{
    uint8_t aEntryPoint[4];
    uint8_t aNintendoLogo[48];
    char aTitle[16];
    char aNewLicenseeCode[2];
    uint8_t SGB_Flag;
    uint8_t CartridgeType;
    uint8_t ROM_Size;
    uint8_t RAM_Size;
    uint8_t DestinationCode;
    uint8_t OldLicenseeCode;
    uint8_t MaskROMVersionNumber;
    uint8_t HeaderChecksum;
    uint8_t aGlobalChecksum[2];
};

struct mem_t
{
    bool BootROMEnabled;
    uint8_t *pBootROM; // BootROM
    uint8_t *pCartridgeROM;
    uint8_t *pCartridgeRAM;
    bool CartridgeRAM_Enabled;

    // Array of pointers on cartridge ROM Banks
    uint8_t *aCartridgeROMBank[MEM_CARTRIDGE_ROM_BANK_MAX];
    uint8_t *aCartridgeRAMBank[MEM_CARTRIDGE_RAM_BANK_MAX];

    // Mapped Banks
    uint8_t ROMIndex0; // [0x0000 - 0x3FFF]
    uint8_t ROMIndex1; // [0x4000 - 0x7FFF]
    uint8_t RAMIndex;  // [0xA000 - 0xBFFF]

    // On board RAM
    uint8_t SRAM[MEM_SRAM_SIZE];
    uint8_t VRAM[MEM_VRAM_SIZE];
    uint8_t OAM_RAM[MEM_OAM_RAM_SIZE];
    uint8_t HRAM[MEM_HRAM_SIZE];
    uint8_t IOPorts[MEM_IO_PORTS_SIZE];

    bool dma_ongoing;

    // Cartridge specs
    uint8_t ROMSize;
    uint8_t RAMSize;

    // Save filename
    char *pSaveFilename;

    mbc_func_t pMBC;

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
            return &mem.aCartridgeROMBank[mem.ROMIndex0][Addr];
        case 0x4000: // Mapped ROM Bank
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return &mem.aCartridgeROMBank[mem.ROMIndex1][Addr - 0x4000];
        case 0x8000: // VRAM
        case 0x9000:
            if (Override || ppu.pReg->STAT_Flags.ModeFlag != STATE_PXL_XFER)
                return &mem.VRAM[Addr - 0x8000];
            return NULL; // Not accessible during Pixel Transfer
        case 0xA000: // Cartridge RAM
        case 0xB000:
            if (mem.CartridgeRAM_Enabled)
                return &mem.aCartridgeRAMBank[mem.RAMIndex][Addr - 0xA000];
            return NULL;
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

void mem_init(void)
{
    // Init BootROM location
    mem.pBootROM = (uint8_t *) 0x08100000;
    mem.BootROMEnabled = true;

    // Init Cartridge ROM banks location
    mem.pCartridgeROM = NULL;
    memset(mem.aCartridgeROMBank, 0, MEM_CARTRIDGE_ROM_BANK_MAX * sizeof(uint8_t *));
    mem.aCartridgeROMBank[0] = (uint8_t *) 0x08110000;
    mem.aCartridgeROMBank[1] = (uint8_t *) 0x08118000;

    // Init Cartridge RAM
    mem.pCartridgeRAM = NULL;
    memset(mem.aCartridgeRAMBank, 0, MEM_CARTRIDGE_RAM_BANK_MAX * sizeof(uint8_t *));

    // Map memory
    mem.ROMIndex0 = 0;
    mem.ROMIndex1 = 1;
    mem.RAMIndex = 0;

    // Cartridge
    mem.CartridgeRAM_Enabled = false;
    mem.ROMSize = 0;
    mem.RAMSize = 0;

    mem.pSaveFilename = NULL;

    // MBC
    mem.pMBC = NULL;
}

void mem_deinit(void)
{
    if (mem.pBootROM)
        free(mem.pBootROM);

    if (mem.pCartridgeROM)
        free(mem.pCartridgeROM);

    if (mem.pCartridgeRAM)
        free(mem.pCartridgeRAM);
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
    if (Addr >= 0x8000) // Protect ROM sections
    {
        uint8_t *pU8 = (uint8_t *) mem_translation(Addr, false);
        if (NULL != pU8)
        {
            if (Addr >= 0xFF00 && Addr <= 0xFF7F && (aIOPortsActionOnWrMap[Addr - 0xFF00] == true))
                action_on_w8(Addr, Value, pU8);
            else
            {
                *pU8 = Value;
            }
        }
    }
    else if (mem.pMBC != NULL)
    {
        mem.pMBC(Addr, Value);
    }
}

void mem_write_u16(uint16_t Addr, uint16_t Value)
{
    if (Addr >= 0x8000) // Protect ROM sections
    {
        uint16_t *pU16 = (uint16_t *) mem_translation(Addr, false);
        if (NULL != pU16)
            *pU16 = Value;
    }
    else
    {
        // TODO: Implement MBC
    }
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

static inline int read_file(char *pFilename, uint8_t **pMem, size_t *pSize)
{
    FILE *pFile = fopen(pFilename, "rb");
    if (NULL == pFile)
    {
        printf("Cannot open file: %s\n", pFilename);
        return EXIT_FAILURE;
    }

    // Get the size of the game
    fseek(pFile, 0L, SEEK_END);
    size_t file_size = ftell(pFile);
    rewind(pFile);

    // Allocate Game memory
    *pMem = (uint8_t*) malloc(file_size);
    if (NULL == *pMem)
    {
        printf("Cannot allocate memory for file (%lu B)\n", file_size);
        return EXIT_FAILURE;
    }

    // Load Game
    size_t read_size = fread(*pMem, sizeof(uint8_t), file_size, pFile);
    if (read_size != file_size)
    {
        printf("Error loading file: %d\n", (int) read_size);
        return EXIT_FAILURE;
    }
    fclose(pFile);

    *pSize = file_size;

    return EXIT_SUCCESS;
}

int mem_load_bootrom(char *pFilename)
{
    if (NULL == pFilename)
        return EXIT_FAILURE;

    size_t boot_size;

    if (read_file(pFilename, &mem.pBootROM, &boot_size))
    {
        printf("Error: Cannot load BOOT ROM\n");
        return EXIT_FAILURE;
    }

    if (BOOT_ROM_SIZE != boot_size)
    {
        printf("Error: BOOT ROM wrong size\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static inline uint16_t get_romsize(uint8_t code)
{
    if (code > 0x08)
    {
        printf("Unknown ROM size : %02x\n", code);
        return 0;
    }
    return 0x02 << code;
}

static inline uint8_t get_ramsize(uint8_t code)
{
    switch (code)
    {
        case 0x00: // No RAM
            return 0;
        case 0x02: // 8 KiB
            return 1;
        case 0x03: // 32 KiB
            return 4;
        case 0x04: // 128 KiB
            return 16;
        case 0x05: // 64 KiB
            return 8;
        default:
            printf("Unknown RAM size : %02x\n", code);
            return 0;
    }
}

int mem_load_gamerom(char *pFilename)
{
    if (NULL == pFilename)
        return EXIT_FAILURE;

    size_t game_size;

    if (read_file(pFilename, &mem.pCartridgeROM, &game_size))
    {
        printf("Error: Cannot load Game ROM\n");
        return EXIT_FAILURE;
    }

    struct cartridge_header_t *pHeader = (struct cartridge_header_t*) &mem.pCartridgeROM[0x100];

    mem.ROMSize = pHeader->ROM_Size;
    mem.RAMSize = pHeader->RAM_Size;

    uint16_t rom_bank = get_romsize(mem.ROMSize);
    uint8_t ram_bank = get_ramsize(mem.RAMSize);

    printf("Title : %.16s\n", pHeader->aTitle);
    printf("MBC type: %02x\n", pHeader->CartridgeType);
    printf("ROM Size : %d (%02x)\n", rom_bank, pHeader->ROM_Size);
    printf("RAM Size : %d (%02x)\n", ram_bank, pHeader->RAM_Size);

    for (int bank = 0 ; bank < rom_bank ; bank++)
    {
        mem.aCartridgeROMBank[bank] = &mem.pCartridgeROM[MEM_CARTRIDGE_ROM_BANK_SIZE * bank];
    }

    // Alloc Cartridge RAM
    if (ram_bank > 0)
    {
        mem.pCartridgeRAM = (uint8_t*) malloc(ram_bank * MEM_CARTRIDGE_RAM_BANK_SIZE);

        for (int bank = 0 ; bank < ram_bank ; bank++)
        {
            mem.aCartridgeRAMBank[bank] = &mem.pCartridgeRAM[MEM_CARTRIDGE_RAM_BANK_SIZE * bank];
        }
    }

    // Select MBC
    mem.pMBC = mbc_get_callback(pHeader->CartridgeType);

    return EXIT_SUCCESS;
}

void mem_set_rombank0(uint8_t Bank)
{
    mem.ROMIndex0 = Bank;
}

void mem_set_rombank1(uint8_t Bank)
{
    mem.ROMIndex1 = Bank;
}

void mem_set_rambank(bool Enable, uint8_t Bank)
{
    mem.CartridgeRAM_Enabled = Enable;
    mem.RAMIndex = Bank;
}

void mem_ram_load(void)
{

}

void mem_ram_save(void)
{

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
