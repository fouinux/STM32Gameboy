/*
 * core.c
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/cpu/core.h>
#include <gameboy/cpu/memory.h>
#include <gameboy/cpu/opcode.h>
#include <gameboy/cpu/opcode_cb.h>

// Also defined as global
struct core_reg_t core_reg;

void core_execute(void)
{
	// Read opcode
	uint8_t opcode = mem_read_u8(core_reg.PC);

    // Execute opcode
	if (opcode == 0xCB)
	    opcodeCbList[opcode].func();
	else
	    opcodeList[opcode].func();

	// Update PC
	if (true == opcodeList[opcode].updatePC)
		core_reg.PC += opcodeList[opcode].length;
}
