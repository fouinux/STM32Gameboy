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

// Load literally 16bits
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

// Load literally 8bits
static void op_LD_L8(uint8_t opcode)
{
	uint8_t *pReg = NULL;

	switch (opcode)
	{
	case 0x06: // LD B, d8
		pReg = &core_reg.B;
		break;
	case 0x0E: // LD C, d8
		pReg = &core_reg.C;
		break;
	case 0x16: // LD D, d8
		pReg = &core_reg.D;
		break;
	case 0x1E: // LD E, d8
		pReg = &core_reg.E;
		break;
	case 0x26: // LD H, d8
		pReg = &core_reg.H;
		break;
	case 0x2E: // LD L, d8
		pReg = &core_reg.L;
		break;
	case 0x3E: // LD A, d8
		pReg = &core_reg.A;
		break;
	}

	*pReg = mem_read_u8(core_reg.PC + 1);
}

// Load Address with A
void op_LD_Addr_A(uint8_t opcode)
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

// Load A with Address
void op_LD_A_Addr(uint8_t opcode)
{
	switch (opcode)
	{
	case 0x0A: // LD A, (BC)
		core_reg.A = mem_read_u8(core_reg.BC);
		break;
	case 0x1A: // LD A, (DE)
		core_reg.A = mem_read_u8(core_reg.DE);
		break;
	case 0x2A: // LD A, (HL+)
		core_reg.A = mem_read_u8(core_reg.HL++);
		break;
	case 0x3A: // LD A, (HL-)
		core_reg.A = mem_read_u8(core_reg.HL--);
		break;
	}
}

// Increment 16bits registers
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
	case 0x23: // INC HL
		core_reg.HL++;
		break;
	case 0x33: // INC SP
		core_reg.SP++;
		break;
	}
}

// Decrement 16bits registers
static void op_DEC16(uint8_t opcode)
{
	switch (opcode)
	{
	case 0x0B: // DEC BC
		core_reg.BC++;
		break;
	case 0x1B: // DEC DE
		core_reg.DE++;
		break;
	case 0x2B: // DEC HL
		core_reg.D++;
		break;
	case 0x3B: // DEC SP
		core_reg.BC++;
		break;
	}
}

// Increment 8bits registers
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

// Decrement 8bits registers
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

// Add 16bits
static void op_ADD16(uint8_t opcode)
{
	uint32_t result;

	// Reset N H C flags
	core_reg.F &= 0x80;

	switch (opcode)
	{
	case 0x09: // ADD HL, BC
		result = (uint32_t) core_reg.HL + (uint32_t) core_reg.BC;
		if ((core_reg.HL & 0xFFF) + (core_reg.BC & 0xFFF) > 0xFFF)
			core_reg.Flags.H = 1;
		break;
	case 0x19: // ADD HL, DE
		result = (uint32_t) core_reg.HL + (uint32_t) core_reg.DE;
		if ((core_reg.HL & 0xFFF) + (core_reg.DE & 0xFFF) > 0xFFF)
			core_reg.Flags.H = 1;
		break;
	case 0x29: // ADD HL, HL
		result = (uint32_t) core_reg.HL + (uint32_t) core_reg.HL;
		if ((core_reg.HL & 0xFFF) + (core_reg.HL & 0xFFF) > 0xFFF)
			core_reg.Flags.H = 1;
		break;
	case 0x39: // ADD HL, SP
		result = (uint32_t) core_reg.HL + (uint32_t) core_reg.SP;
		if ((core_reg.HL & 0xFFF) + (core_reg.SP & 0xFFF) > 0xFFF)
			core_reg.Flags.H = 1;
		break;
	}

	core_reg.HL = result & 0xFFFF;

	// Update Carry flag
	if (result > 0xFFFF)
		core_reg.Flags.C = 1;

}

struct opcode_t opcodeList[256] =
{
	{op_NOP,		1,	4},		// 0x00
	{op_LD_L16,		3,	12},	// 0x01
	{op_LD_Addr_A, 	1,	8},		// 0x02
	{op_INC16, 		1,	8},		// 0x03
	{op_INC8, 		1,	4},		// 0x04
	{op_DEC8, 		1,	4},		// 0x05
	{op_LD_L8, 		2,	8},		// 0x06
	{NULL,			0,	0},		// 0x07 TODO
	{NULL,			0,	0},		// 0x08 TODO
	{op_ADD16, 		1,	8},		// 0x09
	{op_LD_A_Addr, 	1,	8},		// 0x0A
	{op_DEC16, 		1,	8},		// 0x0B
	{op_INC8, 		1,	4},		// 0x0C
	{op_DEC8, 		1,	4},		// 0x0D
	{op_LD_L8, 		2,	8},		// 0x0E
	{NULL,			0,	0},		// 0x0F TODO
	{op_LD_L16,		3,	12},	// 0x11
	{op_LD_Addr_A, 	1,	8},		// 0x12
	{op_INC16, 		1,	8},		// 0x13
	{op_INC8, 		1,	4},		// 0x14
	{op_DEC8, 		1,	4},		// 0x15
	{op_LD_L8, 		2,	8},		// 0x16

	{op_ADD16, 		1,	8},		// 0x19
	{op_LD_A_Addr, 	1,	8},		// 0x1A
	{op_DEC16, 		1,	8},		// 0x1B
	{op_INC8, 		1,	4},		// 0x1C
	{op_DEC8, 		1,	4},		// 0x1D
	{op_LD_L8, 		2,	8},		// 0x1E
	{NULL,			0,	0},		// 0x1F TODO
	{op_LD_L16,		3,	12},	// 0x21
	{op_LD_Addr_A, 	1,	8},		// 0x22
	{op_INC16, 		1,	8},		// 0x23
	{op_INC8, 		1,	4},		// 0x24
	{op_DEC8, 		1,	4},		// 0x25
	{op_LD_L8, 		2,	8},		// 0x26

	{op_ADD16, 		1,	8},		// 0x29
	{op_LD_A_Addr, 	1,	8},		// 0x2A
	{op_DEC16, 		1,	8},		// 0x2B
	{op_INC8, 		1,	4},		// 0x2C
	{op_DEC8, 		1,	4},		// 0x2D
	{op_LD_L8, 		2,	8},		// 0x2E
	{NULL,			0,	0},		// 0x2F TODO
	{op_LD_L16,		3,	12},	// 0x31
	{op_LD_Addr_A, 	1,	8},		// 0x32
	{op_INC16, 		1,	8},		// 0x33
	{op_INC8, 		1,	4},		// 0x34
	{op_DEC8, 		1,	4},		// 0x35

	{op_ADD16, 		1,	8},		// 0x39
	{op_LD_A_Addr, 	1,	8},		// 0x3A
	{op_DEC16, 		1,	8},		// 0x3B
	{op_INC8, 		1,	4},		// 0x3C
	{op_DEC8, 		1,	4},		// 0x3D
	{op_LD_L8, 		2,	8},		// 0x3E
};
