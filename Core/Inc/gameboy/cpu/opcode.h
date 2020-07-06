/*
 * opcode.h
 *
 *  Created on: Jul 6, 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_CPU_OPCODE_H_
#define INC_GAMEBOY_CPU_OPCODE_H_

#include <stdint.h>

struct opcode_t
{
	void (*func)(uint8_t);
	uint8_t length;
	uint8_t cycles;
};


#endif /* INC_GAMEBOY_CPU_OPCODE_H_ */
