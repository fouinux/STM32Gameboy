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

//////////////////////
//    8-bit Load    //
//////////////////////

// Macro: LD r1, r2
#define MACRO_OP_LD_r1_r2(r1, r2) \
static void op_LD_##r1##_##r2(uint8_t optcode) \
{ \
	core_reg.r1 = core_reg.r2; \
}

MACRO_OP_LD_r1_r2(B, B);	// LD B, B
MACRO_OP_LD_r1_r2(B, C);	// LD B, C
MACRO_OP_LD_r1_r2(B, D);	// LD B, D
MACRO_OP_LD_r1_r2(B, E);	// LD B, E
MACRO_OP_LD_r1_r2(B, H);	// LD B, H
MACRO_OP_LD_r1_r2(B, L);	// LD B, L
MACRO_OP_LD_r1_r2(B, A);	// LD B, A

MACRO_OP_LD_r1_r2(C, B);	// LD C, B
MACRO_OP_LD_r1_r2(C, C);	// LD C, C
MACRO_OP_LD_r1_r2(C, D);	// LD C, D
MACRO_OP_LD_r1_r2(C, E);	// LD C, E
MACRO_OP_LD_r1_r2(C, H);	// LD C, H
MACRO_OP_LD_r1_r2(C, L);	// LD C, L
MACRO_OP_LD_r1_r2(C, A);	// LD C, A

MACRO_OP_LD_r1_r2(D, B);	// LD D, B
MACRO_OP_LD_r1_r2(D, C);	// LD D, C
MACRO_OP_LD_r1_r2(D, D);	// LD D, D
MACRO_OP_LD_r1_r2(D, E);	// LD D, E
MACRO_OP_LD_r1_r2(D, H);	// LD D, H
MACRO_OP_LD_r1_r2(D, L);	// LD D, L
MACRO_OP_LD_r1_r2(D, A);	// LD D, A

MACRO_OP_LD_r1_r2(E, B);	// LD E, B
MACRO_OP_LD_r1_r2(E, C);	// LD E, C
MACRO_OP_LD_r1_r2(E, D);	// LD E, D
MACRO_OP_LD_r1_r2(E, E);	// LD E, E
MACRO_OP_LD_r1_r2(E, H);	// LD E, H
MACRO_OP_LD_r1_r2(E, L);	// LD E, L
MACRO_OP_LD_r1_r2(E, A);	// LD E, A

MACRO_OP_LD_r1_r2(H, B);	// LD H, B
MACRO_OP_LD_r1_r2(H, C);	// LD H, C
MACRO_OP_LD_r1_r2(H, D);	// LD H, D
MACRO_OP_LD_r1_r2(H, E);	// LD H, E
MACRO_OP_LD_r1_r2(H, H);	// LD H, H
MACRO_OP_LD_r1_r2(H, L);	// LD H, L
MACRO_OP_LD_r1_r2(H, A);	// LD H, A

MACRO_OP_LD_r1_r2(L, B);	// LD L, B
MACRO_OP_LD_r1_r2(L, C);	// LD L, C
MACRO_OP_LD_r1_r2(L, D);	// LD L, D
MACRO_OP_LD_r1_r2(L, E);	// LD L, E
MACRO_OP_LD_r1_r2(L, H);	// LD L, H
MACRO_OP_LD_r1_r2(L, L);	// LD L, L
MACRO_OP_LD_r1_r2(L, A);	// LD L, A

MACRO_OP_LD_r1_r2(A, B);	// LD A, B
MACRO_OP_LD_r1_r2(A, C);	// LD A, C
MACRO_OP_LD_r1_r2(A, D);	// LD A, D
MACRO_OP_LD_r1_r2(A, E);	// LD A, E
MACRO_OP_LD_r1_r2(A, H);	// LD A, H
MACRO_OP_LD_r1_r2(A, L);	// LD A, L
MACRO_OP_LD_r1_r2(A, A);	// LD A, A

// Macro: LD r1, (HL)
#define MACRO_OP_LD_r1_HL(r1) \
static void op_LD_##r1##_HL(uint8_t optcode) \
{ \
	core_reg.r1 = mem_read_u8(core_reg.HL); \
}

MACRO_OP_LD_r1_HL(B);		// LD B, (HL)
MACRO_OP_LD_r1_HL(C);		// LD C, (HL)
MACRO_OP_LD_r1_HL(D);		// LD D, (HL)
MACRO_OP_LD_r1_HL(E);		// LD E, (HL)
MACRO_OP_LD_r1_HL(H);		// LD H, (HL)
MACRO_OP_LD_r1_HL(L);		// LD L, (HL)
MACRO_OP_LD_r1_HL(A);		// LD A, (HL)

// Macro: LD (HL), r1
#define MACRO_OP_LD_HL_r1(r1) \
static void op_LD_HL_##r1(uint8_t optcode) \
{ \
	mem_write_u8(core_reg.HL, core_reg.r1); \
}

MACRO_OP_LD_HL_r1(B);		// LD (HL), B
MACRO_OP_LD_HL_r1(C);		// LD (HL), C
MACRO_OP_LD_HL_r1(D);		// LD (HL), D
MACRO_OP_LD_HL_r1(E);		// LD (HL), E
MACRO_OP_LD_HL_r1(H);		// LD (HL), H
MACRO_OP_LD_HL_r1(L);		// LD (HL), L
MACRO_OP_LD_HL_r1(A);		// LD (HL), A


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

// Macro: ADD A, r1
#define MACRO_OP_ADD_A_r1(r1) \
static void op_ADD_A_##r1(uint8_t optcode) \
{ \
	uint16_t t = core_reg.A + core_reg.r1; \
	core_reg.F = 0; \
	core_reg.Flags.Z = ((t & 0xFF) == 0);\
	core_reg.Flags.H = ((core_reg.A & 0xF) + (core_reg.r1 & 0xF) > 0xF); \
	core_reg.Flags.C = (t > 0xFF); \
	core_reg.A = t & 0xFF; \
}

MACRO_OP_ADD_A_r1(B);	// ADD A, B
MACRO_OP_ADD_A_r1(C);	// ADD A, C
MACRO_OP_ADD_A_r1(D);	// ADD A, D
MACRO_OP_ADD_A_r1(E);	// ADD A, E
MACRO_OP_ADD_A_r1(H);	// ADD A, H
MACRO_OP_ADD_A_r1(L);	// ADD A, L
MACRO_OP_ADD_A_r1(A);	// ADD A, A

// ADD A, (HL)
static void op_ADD_A_HL(uint8_t optcode)
{
	uint8_t reg = mem_read_u8(core_reg.HL);
	uint16_t t = core_reg.A + reg;
	core_reg.F = 0;
	core_reg.Flags.Z = ((t & 0xFF) == 0);
	core_reg.Flags.H = ((core_reg.A & 0xF) + (reg & 0xF) > 0xF);
	core_reg.Flags.C = (t > 0xFF);
	core_reg.A = t & 0xFF;
}

// Macro: ADC A, r1
#define MACRO_OP_ADC_A_r1(r1) \
static void op_ADC_A_##r1(uint8_t optcode) \
{ \
	uint16_t t = core_reg.A + core_reg.r1 + core_reg.Flags.C; \
	core_reg.F = 0; \
	core_reg.Flags.Z = ((t & 0xFF) == 0);\
	core_reg.Flags.H = ((core_reg.A & 0xF) + (core_reg.r1 & 0xF) > 0xF); \
	core_reg.Flags.C = (t > 0xFF); \
	core_reg.A = t & 0xFF; \
}

MACRO_OP_ADC_A_r1(B);	// ADC A, B
MACRO_OP_ADC_A_r1(C);	// ADC A, C
MACRO_OP_ADC_A_r1(D);	// ADC A, D
MACRO_OP_ADC_A_r1(E);	// ADC A, E
MACRO_OP_ADC_A_r1(H);	// ADC A, H
MACRO_OP_ADC_A_r1(L);	// ADC A, L
MACRO_OP_ADC_A_r1(A);	// ADC A, A

// ADC A, (HL)
static void op_ADC_A_HL(uint8_t optcode)
{
	uint8_t reg = mem_read_u8(core_reg.HL);
	uint16_t t = core_reg.A + reg +  core_reg.Flags.C;
	core_reg.F = 0;
	core_reg.Flags.Z = ((t & 0xFF) == 0);
	core_reg.Flags.H = ((core_reg.A & 0xF) + (reg & 0xF) > 0xF);
	core_reg.Flags.C = (t > 0xFF);
	core_reg.A = t & 0xFF;
}

// Macro: SUB A, r1
#define MACRO_OP_SUB_A_r1(r1) \
static void op_SUB_A_##r1(uint8_t optcode) \
{ \
	int16_t t = core_reg.A - core_reg.r1; \
	core_reg.F = 0x40; \
	core_reg.Flags.Z = ((t & 0xFF) == 0);\
	core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) core_reg.r1 & 0xF) < 0); \
	core_reg.Flags.C = (t < 0); \
	core_reg.A = t & 0xFF; \
}

MACRO_OP_SUB_A_r1(B);	// SUB A, B
MACRO_OP_SUB_A_r1(C);	// SUB A, C
MACRO_OP_SUB_A_r1(D);	// SUB A, D
MACRO_OP_SUB_A_r1(E);	// SUB A, E
MACRO_OP_SUB_A_r1(H);	// SUB A, H
MACRO_OP_SUB_A_r1(L);	// SUB A, L
MACRO_OP_SUB_A_r1(A);	// SUB A, A

static void op_SUB_A_HL(uint8_t optcode)
{
	uint8_t reg = mem_read_u8(core_reg.HL);
	int16_t t = core_reg.A - reg;
	core_reg.F = 0x40;
	core_reg.Flags.Z = ((t & 0xFF) == 0);
	core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
	core_reg.Flags.C = (t < 0);
	core_reg.A = t & 0xFF;
}


// Macro: SBC A, r1
#define MACRO_OP_SBC_A_r1(r1) \
static void op_SBC_A_##r1(uint8_t optcode) \
{ \
	int16_t t = core_reg.A - core_reg.r1 - core_reg.Flags.C; \
	core_reg.F = 0x40; \
	core_reg.Flags.Z = ((t & 0xFF) == 0);\
	core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) core_reg.r1 & 0xF) < 0); \
	core_reg.Flags.C = (t < 0); \
	core_reg.A = t & 0xFF; \
}

MACRO_OP_SBC_A_r1(B);	// SBC A, B
MACRO_OP_SBC_A_r1(C);	// SBC A, C
MACRO_OP_SBC_A_r1(D);	// SBC A, D
MACRO_OP_SBC_A_r1(E);	// SBC A, E
MACRO_OP_SBC_A_r1(H);	// SBC A, H
MACRO_OP_SBC_A_r1(L);	// SBC A, L
MACRO_OP_SBC_A_r1(A);	// SBC A, A

static void op_SBC_A_HL(uint8_t optcode)
{
	uint8_t reg = mem_read_u8(core_reg.HL);
	int16_t t = core_reg.A - reg - core_reg.Flags.C;
	core_reg.F = 0x40;
	core_reg.Flags.Z = ((t & 0xFF) == 0);
	core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
	core_reg.Flags.C = (t < 0);
	core_reg.A = t & 0xFF;
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

	{op_LD_B_B,		1,	4,	true},		// 0x40
	{op_LD_B_C,		1,	4,	true},		// 0x41
	{op_LD_B_D,		1,	4,	true},		// 0x42
	{op_LD_B_E,		1,	4,	true},		// 0x43
	{op_LD_B_H,		1,	4,	true},		// 0x44
	{op_LD_B_L,		1,	4,	true},		// 0x45
	{op_LD_B_HL,	1,	8,	true},		// 0x46
	{op_LD_B_A,		1,	4,	true},		// 0x47
	{op_LD_C_B,		1,	4,	true},		// 0x48
	{op_LD_C_C,		1,	4,	true},		// 0x49
	{op_LD_C_D,		1,	4,	true},		// 0x4A
	{op_LD_C_E,		1,	4,	true},		// 0x4B
	{op_LD_C_H,		1,	4,	true},		// 0x4C
	{op_LD_C_L,		1,	4,	true},		// 0x4D
	{op_LD_C_HL,	1,	8,	true},		// 0x4E
	{op_LD_C_A,		1,	4,	true},		// 0x4F
	{op_LD_D_B,		1,	4,	true},		// 0x50
	{op_LD_D_C,		1,	4,	true},		// 0x51
	{op_LD_D_D,		1,	4,	true},		// 0x52
	{op_LD_D_E,		1,	4,	true},		// 0x53
	{op_LD_D_H,		1,	4,	true},		// 0x54
	{op_LD_D_L,		1,	4,	true},		// 0x55
	{op_LD_D_HL,	1,	8,	true},		// 0x56
	{op_LD_D_A,		1,	4,	true},		// 0x57
	{op_LD_E_B,		1,	4,	true},		// 0x58
	{op_LD_E_C,		1,	4,	true},		// 0x59
	{op_LD_E_D,		1,	4,	true},		// 0x5A
	{op_LD_E_E,		1,	4,	true},		// 0x5B
	{op_LD_E_H,		1,	4,	true},		// 0x5C
	{op_LD_E_L,		1,	4,	true},		// 0x5D
	{op_LD_E_HL,	1,	8,	true},		// 0x5E
	{op_LD_E_A,		1,	4,	true},		// 0x5F
	{op_LD_H_B,		1,	4,	true},		// 0x60
	{op_LD_H_C,		1,	4,	true},		// 0x61
	{op_LD_H_D,		1,	4,	true},		// 0x62
	{op_LD_H_E,		1,	4,	true},		// 0x63
	{op_LD_H_H,		1,	4,	true},		// 0x64
	{op_LD_H_L,		1,	4,	true},		// 0x65
	{op_LD_H_HL,	1,	8,	true},		// 0x66
	{op_LD_H_A,		1,	4,	true},		// 0x67
	{op_LD_L_B,		1,	4,	true},		// 0x68
	{op_LD_L_C,		1,	4,	true},		// 0x69
	{op_LD_L_D,		1,	4,	true},		// 0x6A
	{op_LD_L_E,		1,	4,	true},		// 0x6B
	{op_LD_L_H,		1,	4,	true},		// 0x6C
	{op_LD_L_L,		1,	4,	true},		// 0x6D
	{op_LD_L_HL,	1,	8,	true},		// 0x6E
	{op_LD_L_A,		1,	4,	true},		// 0x6F
	{op_LD_HL_B,	1,	8,	true},		// 0x70
	{op_LD_HL_C,	1,	8,	true},		// 0x71
	{op_LD_HL_D,	1,	8,	true},		// 0x72
	{op_LD_HL_E,	1,	8,	true},		// 0x73
	{op_LD_HL_H,	1,	8,	true},		// 0x74
	{op_LD_HL_L,	1,	8,	true},		// 0x75

	{NULL,			1,	4,	true},		// 0x76 // TODO HALT

	{op_LD_HL_A,	1,	8,	true},		// 0x77
	{op_LD_A_B,		1,	4,	true},		// 0x78
	{op_LD_A_C,		1,	4,	true},		// 0x79
	{op_LD_A_D,		1,	4,	true},		// 0x7A
	{op_LD_A_E,		1,	4,	true},		// 0x7B
	{op_LD_A_H,		1,	4,	true},		// 0x7C
	{op_LD_A_L,		1,	4,	true},		// 0x7D
	{op_LD_A_HL,	1,	8,	true},		// 0x7E
	{op_LD_A_A,		1,	4,	true},		// 0x7F
	{op_ADD_A_B,	1,	4,	true},		// 0x80
	{op_ADD_A_C,	1,	4,	true},		// 0x81
	{op_ADD_A_D,	1,	4,	true},		// 0x82
	{op_ADD_A_E,	1,	4,	true},		// 0x83
	{op_ADD_A_H,	1,	4,	true},		// 0x84
	{op_ADD_A_L,	1,	4,	true},		// 0x85
	{op_ADD_A_HL,	1,	8,	true},		// 0x86
	{op_ADD_A_A,	1,	4,	true},		// 0x87
	{op_ADC_A_B,	1,	4,	true},		// 0x88
	{op_ADC_A_C,	1,	4,	true},		// 0x89
	{op_ADC_A_D,	1,	4,	true},		// 0x8A
	{op_ADC_A_E,	1,	4,	true},		// 0x8B
	{op_ADC_A_H,	1,	4,	true},		// 0x8C
	{op_ADC_A_L,	1,	4,	true},		// 0x8D
	{op_ADC_A_HL,	1,	8,	true},		// 0x8E
	{op_ADC_A_A,	1,	4,	true},		// 0x8F
	{op_SUB_A_B,	1,	4,	true},		// 0x90
	{op_SUB_A_C,	1,	4,	true},		// 0x91
	{op_SUB_A_D,	1,	4,	true},		// 0x92
	{op_SUB_A_E,	1,	4,	true},		// 0x93
	{op_SUB_A_H,	1,	4,	true},		// 0x94
	{op_SUB_A_L,	1,	4,	true},		// 0x95
	{op_SUB_A_HL,	1,	8,	true},		// 0x96
	{op_SUB_A_A,	1,	4,	true},		// 0x97
	{op_SBC_A_B,	1,	4,	true},		// 0x98
	{op_SBC_A_C,	1,	4,	true},		// 0x99
	{op_SBC_A_D,	1,	4,	true},		// 0x9A
	{op_SBC_A_E,	1,	4,	true},		// 0x9B
	{op_SBC_A_H,	1,	4,	true},		// 0x9C
	{op_SBC_A_L,	1,	4,	true},		// 0x9D
	{op_SBC_A_HL,	1,	8,	true},		// 0x9E
	{op_SBC_A_A,	1,	4,	true},		// 0x9F
};
