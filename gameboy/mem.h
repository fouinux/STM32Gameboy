/*
 * memory.h
 *
 *  Created on: Jul 6, 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_MEM_H_
#define INC_GAMEBOY_MEM_H_

#include <stdint.h>
#include <stdbool.h>

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

void mem_init();
uint8_t mem_read_u8(uint16_t Addr);
int8_t mem_read_s8(uint16_t Addr);
uint16_t mem_read_u16(uint16_t Addr);
void mem_write_u8(uint16_t Addr, uint8_t Value);
void mem_write_u16(uint16_t Addr, uint16_t Value);
uint8_t* mem_get_register(enum IOPorts_reg reg);

uint8_t* mem_get_oam_ram(void);
uint8_t* mem_get_vram(void);

void mem_set_bootrom(uint8_t *pBootROM);
void mem_set_gamerom(uint8_t *pGameROM, uint8_t index);

#endif /* INC_GAMEBOY_MEM_H_ */
