/*
 * memory.c
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/cpu/memory.h>

#include <stdio.h>

struct memory_map_t
{
	uint8_t SRAM[8192];
	uint8_t VRAM[8192];
} memory_map;

static void* mem_translation(uint16_t Addr)
{
	if (Addr < 0x4000) // TODO ROM Bank #0
		return NULL;

	if (Addr < 0x8000) // TODO Switchable ROM Bank
		return NULL;

	if (Addr < 0xA000) // VRAM
		return &memory_map.VRAM[Addr - 0xA000];

	if (Addr < 0xC000) // TODO Switchable RAM Bank
		return NULL;

	if (Addr < 0xE000) // SRAM
		return &memory_map.SRAM[Addr - 0xE000];

	if (Addr < 0xFE00) // Echo of SRAM
		return &memory_map.SRAM[Addr - 0xFE00];

	// TODO I/O ports
	return NULL;
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
