/*
 * memory.c
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/memory.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MEM_CARTRIDGE_ROM_BANK_MAX      128 // 128 * 16 kiB = 2MiB
#define MEM_CARTRIDGE_RAM_BANK_MAX      16  // 16 * 8 kiB = 128kiB

#define MEM_SRAM_SIZE                   8192 // 8 kiB
#define MEM_VRAM_SIZE                   8192 // 8 kiB

struct memory_map_t
{
    bool BootROM; // BootROM is mapped
    uint8_t *pBootROM; // BootROM

    // Array of pointers on cartridge ROM Banks
    uint8_t *aCartridgeROMBank[MEM_CARTRIDGE_ROM_BANK_MAX];

    // Mapped Banks
    uint8_t *pMappedROMBank; // [0x4000 - 0x8000]
    uint8_t *pMappedRAMBank; // [0xA000 - 0xC000]

    // On board RAM
	uint8_t SRAM[MEM_SRAM_SIZE];
	uint8_t VRAM[MEM_VRAM_SIZE];

} memory_map;


static void* mem_translation(uint16_t Addr)
{
    if (Addr < 0x100 && memory_map.BootROM)
    {
        return &memory_map.pBootROM[Addr];
    }

	if (Addr < 0x4000) // ROM Bank #0
		return &memory_map.aCartridgeROMBank[0][Addr];

	if (Addr < 0x8000) // Mapped ROM Bank
		return &memory_map.pMappedROMBank[Addr - 0x8000];

	if (Addr < 0xA000) // VRAM
		return &memory_map.VRAM[Addr - 0xA000];

	if (Addr < 0xC000) // TODO External RAM Bank
		return &memory_map.pMappedRAMBank[Addr - 0xC000];

	if (Addr < 0xE000) // SRAM
		return &memory_map.SRAM[Addr - 0xE000];

	if (Addr < 0xFE00) // Echo of SRAM
		return &memory_map.SRAM[Addr - 0xFE00];

	// TODO I/O ports
	return NULL;
}

void mem_init()
{
    // Init BootROM location
    memory_map.pBootROM = (uint8_t *) 0x08100000;
    memory_map.BootROM = true;

    // Init Cartridge ROM banks location
    memset(memory_map.aCartridgeROMBank, 0, MEM_CARTRIDGE_ROM_BANK_MAX * sizeof(uint8_t *));
    memory_map.aCartridgeROMBank[0] = (uint8_t *) 0x08110000;
    memory_map.aCartridgeROMBank[1] = (uint8_t *) 0x08118000;

    // Map memory
    memory_map.pMappedROMBank = memory_map.aCartridgeROMBank[1];
    memory_map.pMappedRAMBank = NULL;
}

uint8_t mem_read_u8(uint16_t Addr)
{
	return *((uint8_t *) mem_translation(Addr));
}

int8_t mem_read_s8(uint16_t Addr)
{
	return *((int8_t *) mem_translation(Addr));
}

uint16_t mem_read_u16(uint16_t Addr)
{
	return *((uint16_t *) mem_translation(Addr));
}

void mem_write_u8(uint16_t Addr, uint8_t Value)
{
	*((uint8_t *) mem_translation(Addr)) = Value;
}

void mem_write_u16(uint16_t Addr, uint16_t Value)
{
	*((uint16_t *) mem_translation(Addr)) = Value;
}
