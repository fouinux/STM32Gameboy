/*
 * opcode.c
 *
 *  Created on: Jul 6, 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/cpu/opcode.h>
#include <gameboy/cpu/opcode_ll.h>
#include <gameboy/cpu/core.h>
#include <gameboy/cpu/memory.h>

#include <stdio.h>

static void op_NOP(uint8_t opcode)
{
	// Do nothing for one cycle
}

// Load literally 16bits - LD n, d16
static void op_LD_d16(uint8_t opcode)
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

// Load literally 8bits - LD n, d8
static void op_LD_d8(uint8_t opcode)
{
	uint8_t d8 = mem_read_u8(core_reg.PC + 1);

	switch (opcode)
	{
	case 0x06: // LD B, d8
		core_reg.B = d8;
		break;
	case 0x0E: // LD C, d8
		core_reg.C = d8;
		break;
	case 0x16: // LD D, d8
		core_reg.D = d8;
		break;
	case 0x1E: // LD E, d8
		core_reg.E = d8;
		break;
	case 0x26: // LD H, d8
		core_reg.H = d8;
		break;
	case 0x2E: // LD L, d8
		core_reg.L = d8;
		break;
	case 0x36: // LD (HL), d8
		mem_write_u8(core_reg.HL, d8);
		break;
	case 0x3E: // LD A, d8
		core_reg.A = d8;
		break;
	}
}

// Load Address with A - LD (n), A
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

// Load A with Address - LD A, (n)
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

// Load (a16) with SP - LD (a16), SP
void op_LD_a16_SP(uint8_t opcode)
{
	uint16_t a16 = mem_read_u16(core_reg.PC + 1);

	mem_write_u16(a16, core_reg.SP);
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
		result = core_reg.B + 1;
		break;
	case 0x0C: // INC C
		result = core_reg.C + 1;
		break;
	case 0x14: // INC D
		result = core_reg.D + 1;
		break;
	case 0x1C: // INC E
		result = core_reg.E + 1;
		break;
	case 0x24: // INC H
		result = core_reg.H + 1;
		break;
	case 0x2C: // INC L
		result = core_reg.L + 1;
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
		result = core_reg.B - 1;
		break;
	case 0x0D: // INC C
		result = core_reg.C - 1;
		break;
	case 0x15: // INC D
		result = core_reg.D - 1;
		break;
	case 0x1D: // INC E
		result = core_reg.E - 1;
		break;
	case 0x25: // INC H
		result = core_reg.H - 1;
		break;
	case 0x2D: // INC L
		result = core_reg.L - 1;
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

//////////////////////
// Rotates & Shifts //
//////////////////////

// Rotate A Left
static void op_RLCA(uint8_t opcode)
{
	ll_RLC(&core_reg.A);

	// Update flags
	core_reg.Flags.Z = 0;
}

// Rotate A Left through Carry flag
static void op_RLA(uint8_t opcode)
{
	ll_RL(&core_reg.A);

	// Update flags
	core_reg.Flags.Z = 0;
}

// Rotate A Right
static void op_RRCA(uint8_t opcode)
{
	ll_RRC(&core_reg.A);

	// Update flags
	core_reg.Flags.Z = 0;
}

// Rotate A Right through Carry flag
static void op_RRA(uint8_t opcode)
{
	ll_RR(&core_reg.A);

	// Update flags
	core_reg.Flags.Z = 0;
}

//////////////////////
//     Jumps        //
//////////////////////

// Jump relative - JR r8
static void op_JR_n(uint8_t opcode)
{
	int8_t r8 = mem_read_s8(core_reg.PC + 1);
	core_reg.PC += r8;
}

// Jump relative conditional - JR cc, r8
static void op_JR_cc_n(uint8_t opcode)
{
	// TODO Add 4 cycles in case of jump?
	int8_t r8 = mem_read_s8(core_reg.PC + 1);
	bool test;

	switch (opcode)
	{
	case 0x20: // JR NZ, r8
		test = (core_reg.Flags.Z == 0);
		break;
	case 0x28: // JR Z, r8
		test = (core_reg.Flags.Z == 1);
		break;
	case 0x30: // JR NC, r8
		test = (core_reg.Flags.C == 0);
		break;
	case 0x38: // JR C, r8
		test = (core_reg.Flags.C == 1);
		break;
	}

	if (true == test)
		core_reg.PC += r8; // Relative jump
	else
		core_reg.PC += 2; // Next opcode
}

struct opcode_t opcodeList[256] =
{
	{op_NOP,		1,	4,	true},		// 0x00
	{op_LD_d16,		3,	12,	true},		// 0x01
	{op_LD_Addr_A, 	1,	8,	true},		// 0x02
	{op_INC16, 		1,	8,	true},		// 0x03
	{op_INC8, 		1,	4,	true},		// 0x04
	{op_DEC8, 		1,	4,	true},		// 0x05
	{op_LD_d8, 		2,	8,	true},		// 0x06
	{op_RLCA,		1,	4,	true},		// 0x07
	{op_LD_a16_SP,	3,	20,	true},		// 0x08
	{op_ADD16, 		1,	8,	true},		// 0x09
	{op_LD_A_Addr, 	1,	8,	true},		// 0x0A
	{op_DEC16, 		1,	8,	true},		// 0x0B
	{op_INC8, 		1,	4,	true},		// 0x0C
	{op_DEC8, 		1,	4,	true},		// 0x0D
	{op_LD_d8, 		2,	8,	true},		// 0x0E
	{op_RRCA,		1,	4,	true},		// 0x0F
	{op_LD_d16,		3,	12,	true},		// 0x11
	{op_LD_Addr_A, 	1,	8,	true},		// 0x12
	{op_INC16, 		1,	8,	true},		// 0x13
	{op_INC8, 		1,	4,	true},		// 0x14
	{op_DEC8, 		1,	4,	true},		// 0x15
	{op_LD_d8, 		2,	8,	true},		// 0x16
	{op_RLA, 		1,	4,	true},		// 0x17
	{op_JR_n,		2,	12,	false},		// 0x18
	{op_ADD16, 		1,	8,	true},		// 0x19
	{op_LD_A_Addr, 	1,	8,	true},		// 0x1A
	{op_DEC16, 		1,	8,	true},		// 0x1B
	{op_INC8, 		1,	4,	true},		// 0x1C
	{op_DEC8, 		1,	4,	true},		// 0x1D
	{op_LD_d8, 		2,	8,	true},		// 0x1E
	{op_RRA,		1,	4,	true},		// 0x1F
	{op_JR_cc_n,	2,	8,	false},		// 0x20
	{op_LD_d16,		3,	12,	true},		// 0x21
	{op_LD_Addr_A, 	1,	8,	true},		// 0x22
	{op_INC16, 		1,	8,	true},		// 0x23
	{op_INC8, 		1,	4,	true},		// 0x24
	{op_DEC8, 		1,	4,	true},		// 0x25
	{op_LD_d8, 		2,	8,	true},		// 0x26

	{NULL, 			1,	4,	true},		// 0x27 TODO DAA

	{op_JR_cc_n,	2,	8,	false},		// 0x28
	{op_ADD16, 		1,	8,	true},		// 0x29
	{op_LD_A_Addr, 	1,	8,	true},		// 0x2A
	{op_DEC16, 		1,	8,	true},		// 0x2B
	{op_INC8, 		1,	4,	true},		// 0x2C
	{op_DEC8, 		1,	4,	true},		// 0x2D
	{op_LD_d8, 		2,	8,	true},		// 0x2E

	{NULL,			0,	0,	true},		// 0x2F TODO CPL

	{op_JR_cc_n,	2,	8,	false},		// 0x30
	{op_LD_d16,		3,	12,	true},		// 0x31
	{op_LD_Addr_A, 	1,	8,	true},		// 0x32
	{op_INC16, 		1,	8,	true},		// 0x33
	{op_INC8, 		1,	4,	true},		// 0x34
	{op_DEC8, 		1,	4,	true},		// 0x35
	{op_LD_d8, 		1,	8,	true},		// 0x36

	{NULL,			0,	0,	true},		// 0x37 TODO SCF

	{op_JR_cc_n,	2,	8,	false},		// 0x38
	{op_ADD16, 		1,	8,	true},		// 0x39
	{op_LD_A_Addr, 	1,	8,	true},		// 0x3A
	{op_DEC16, 		1,	8,	true},		// 0x3B
	{op_INC8, 		1,	4,	true},		// 0x3C
	{op_DEC8, 		1,	4,	true},		// 0x3D
	{op_LD_d8, 		2,	8,	true},		// 0x3E

	{NULL,			0,	0,	true},		// 0x3F TODO CCF
};
