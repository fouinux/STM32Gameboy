/*
 * opcode.c
 *
 *  Created on: Jul 6, 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/cpu/opcode.h>
#include <gameboy/cpu/core.h>
#include <gameboy/cpu/memory.h>

#include <stdio.h>

static void op_NOP(uint8_t opcode)
{
	// Do nothing for one cycle
}

// Load literally 16 bits
static void op_LD_L16(uint8_t opcode)
{
	uint16_t *pReg = NULL;

	switch (opcode)
	{
	case 0x01: // LD BC, d16
		pReg = &core_reg.BC;
		break;
	case 0x11: // LD DE, d16
		pReg = &core_reg.DE;
		break;
	case 0x21: // LD HL, d16
		pReg = &core_reg.HL;
		break;
	case 0x31: // LD SP, d16
		pReg = &core_reg.SP;
		break;
	}

	*pReg = mem_read_u16(core_reg.PC + 1);
}

// Load literally 8 bits
static void op_LD_L8(uint8_t opcode)
{
	uint8_t *pReg = NULL;

	switch (opcode)
	{
	case 0x05: // LD B, d8
		pReg = &core_reg.B;
		break;
	case 0x0E: // LD C, d8
		pReg = &core_reg.C;
		break;
	case 0x15: // LD D, d8
		pReg = &core_reg.D;
		break;
	case 0x1E: // LD E, d8
		pReg = &core_reg.E;
		break;
	case 0x25: // LD H, d8
		pReg = &core_reg.H;
		break;
	case 0x2E: // LD L, d8
		pReg = &core_reg.L;
		break;
	}

	*pReg = mem_read_u8(core_reg.PC + 1);
}

// Load  16 bits
void op_LD_A(uint8_t opcode)
{
	switch (opcode)
	{
	case 0x02: // LD (BC), A
		mem_write_u8(core_reg.BC, core_reg.A);
		break;
	case 0x12: // LD (DE), A
		mem_write_u8(core_reg.DE, core_reg.A);
		break;
	case 0x22: // LD (HL+), A
		mem_write_u8(core_reg.HL++, core_reg.A);
		break;
	case 0x32: // LD (HL-), A
		mem_write_u8(core_reg.HL--, core_reg.A);
		break;
	}
}

// Increment 16 bits registers
static void op_INC16(uint8_t opcode)
{
	switch (opcode)
	{
	case 0x03: // INC BC
		core_reg.BC++;
		break;
	case 0x13: // INC DE
		core_reg.DE++;
		break;
	case 0x14: // INC D
		core_reg.D++;
		break;
	case 0x1C: // INC D
		core_reg.BC++;
		break;
	}
}

// Increment 8 bits registers
static void op_INC8(uint8_t opcode)
{
	uint8_t result;

	switch (opcode)
	{
	case 0x04: // INC B
		result = ++core_reg.B;
		break;
	case 0x0C: // INC C
		result = ++core_reg.C;
		break;
	case 0x14: // INC D
		result = ++core_reg.D;
		break;
	case 0x1C: // INC E
		result = ++core_reg.E;
		break;
	case 0x24: // INC H
		result = ++core_reg.H;
		break;
	case 0x2C: // INC L
		result = ++core_reg.L;
		break;
	}

	// Update flags
	core_reg.F &= 0x10; // Reset Z N H flags
	if (result == 0)
		core_reg.Flags.Z = 1;
	core_reg.Flags.N = 0;
	if ((result & 0x0F) == 0x00)
		core_reg.Flags.H = 1;
}

// Decrement 8 bits registers
static void op_DEC8(uint8_t opcode)
{
	uint8_t result;

	switch (opcode)
	{
	case 0x05: // INC B
		result = --core_reg.B;
		break;
	case 0x0D: // INC C
		result = --core_reg.C;
		break;
	case 0x15: // INC D
		result = --core_reg.D;
		break;
	case 0x1D: // INC E
		result = --core_reg.E;
		break;
	case 0x25: // INC H
		result = --core_reg.H;
		break;
	case 0x2D: // INC L
		result = --core_reg.L;
		break;
	}

	// Update flags
	core_reg.F &= 0x10; // Reset Z N H flags
	core_reg.Flags.N = 1;
	if (result == 0)
		core_reg.Flags.Z = 1;
	core_reg.Flags.N = 0;
	if ((result & 0x0F) == 0x0F)
		core_reg.Flags.H = 1;
}

struct opcode_t opcodeList[256] =
{
		{op_NOP,	1,	4},		// 0x00
		{op_LD_L16, 3,	12},	// 0x01
		{op_LD_A, 	1,	8},		// 0x02
		{op_INC16, 	1,	8},		// 0x03
		{op_INC8, 	1,	4},		// 0x04
		{op_DEC8, 	1,	4},		// 0x05
		{op_LD_L8, 	2,	8},		// 0x06
};
