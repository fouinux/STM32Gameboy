/*
 * opcode_ll.c
 *
 *  Created on: 7 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/cpu/opcode_ll.h>
#include <gameboy/cpu/opcode.h>
#include <gameboy/cpu/core.h>
#include <gameboy/cpu/memory.h>

#include <stdio.h>

//////////////////////
// Rotates & Shifts //
//////////////////////

// Low level Rotate Left
void ll_RLC(uint8_t *pReg)
{
	*pReg = (*pReg << 1) | (*pReg >> 7);

	// Update flags
	core_reg.F = 0x00; // Reset Z N H C flags
	core_reg.Flags.Z = (*pReg == 0);
	core_reg.Flags.C = *pReg & 0x01;
}

// Low level Rotate Left through Carry flag
void ll_RL(uint8_t *pReg)
{
	uint16_t result = (*pReg << 1) | core_reg.Flags.C;
	*pReg = result & 0xFF;

	// Update flags
	core_reg.F = 0x00; // Reset Z N H C flags
	core_reg.Flags.Z = (*pReg == 0);
	core_reg.Flags.C = (result > 0xFF);
}

// Low level Rotate Right
void ll_RRC(uint8_t *pReg)
{
	// Update flags
	core_reg.F = 0x00; // Reset Z N H C flags
	core_reg.Flags.C = *pReg & 0x01;
	core_reg.Flags.Z = (*pReg == 0);

	*pReg = (*pReg >> 1) | (*pReg << 7);
}

// Low level Rotate Right through Carry flag
void ll_RR(uint8_t *pReg)
{
	uint16_t result = (*pReg >> 1) | (core_reg.Flags.C << 7);

	// Update flags
	core_reg.F = 0x00; // Reset Z N H C flags
	core_reg.Flags.C = *pReg & 0x01;
	core_reg.Flags.Z = (result == 0);

	*pReg = result & 0xFF;
}
