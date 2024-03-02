/*
 * mem.h
 *
 *  Created on: Jul 6, 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_MEM_H_
#define INC_GAMEBOY_MEM_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MEM_CARTRIDGE_ROM_BANK_SIZE     16384 // 16 KiB
#define MEM_CARTRIDGE_ROM_BANK_MAX      512   // 512 * 16 KiB = 8 MiB
#define MEM_CARTRIDGE_RAM_BANK_SIZE     8192  // 8 KiB
#define MEM_CARTRIDGE_RAM_BANK_MAX      16    // 16 * 8 KiB = 128 KiB

enum IOPorts_reg
{
    JOYPAD,
    SERIAL,
    TIMER,
    SOUND,
    PPU,
    BOOT,
    IF,
    IE,
};

void mem_init(void);
void mem_deinit(void);
uint8_t mem_read_u8(uint16_t Addr);
int8_t mem_read_s8(uint16_t Addr);
uint16_t mem_read_u16(uint16_t Addr);
void mem_write_u8(uint16_t Addr, uint8_t Value);
void mem_write_u16(uint16_t Addr, uint16_t Value);
uint8_t* mem_get_register(enum IOPorts_reg reg);

uint8_t* mem_get_oam_ram(void);
uint8_t* mem_get_vram(void);

int mem_load_bootrom(char *pFilename);
int mem_load_gamerom(char *pFilename);

void mem_set_rombank0(uint8_t Bank);
void mem_set_rombank1(uint8_t Bank);
void mem_set_rambank(bool Enable, uint8_t Bank);

int mem_ram_load(void);
int mem_ram_save(void);

void mem_hexdump(const uint16_t Addr, const size_t Size);

#endif /* INC_GAMEBOY_MEM_H_ */
