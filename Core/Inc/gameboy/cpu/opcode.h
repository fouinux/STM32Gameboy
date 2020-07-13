/*
 * opcode.h
 *
 *  Created on: Jul 6, 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_CPU_OPCODE_H_
#define INC_GAMEBOY_CPU_OPCODE_H_

#include <stdint.h>
#include <stdbool.h>

struct opcode_t
{
	void (*func)(void);
	uint8_t length;
	uint8_t cycles;
	bool updatePC;
};

extern struct opcode_t opcodeList[256];


#endif /* INC_GAMEBOY_CPU_OPCODE_H_ */
