/*
 * opcode_cb.c
 *
 *  Created on: 7 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/cpu/opcode_cb.h>
#include <gameboy/cpu/opcode_ll.h>
#include <gameboy/cpu/opcode.h>
#include <gameboy/cpu/core.h>
#include <gameboy/cpu/memory.h>

#include <stdio.h>

// Rotate Left
static void op_RLC(uint8_t opcode)
{
	uint8_t temp;

	switch(opcode)
	{
	case 0x00:
		ll_RLC(&core_reg.B);
		break;
	case 0x01:
		ll_RLC(&core_reg.C);
		break;
	case 0x02:
		ll_RLC(&core_reg.D);
		break;
	case 0x03:
		ll_RLC(&core_reg.E);
		break;
	case 0x04:
		ll_RLC(&core_reg.H);
		break;
	case 0x05:
		ll_RLC(&core_reg.L);
		break;
	case 0x06:
		temp = mem_read_u8(core_reg.HL);
		ll_RLC(&temp);
		mem_write_u8(core_reg.HL, temp);
		break;
	case 0x07:
		ll_RLC(&core_reg.A);
		break;
	}
}

// Rotate Left through Carry flag
static void op_RL(uint8_t opcode)
{
	uint8_t temp;

	switch(opcode)
	{
	case 0x10:
		ll_RL(&core_reg.B);
		break;
	case 0x11:
		ll_RL(&core_reg.C);
		break;
	case 0x12:
		ll_RL(&core_reg.D);
		break;
	case 0x13:
		ll_RL(&core_reg.E);
		break;
	case 0x14:
		ll_RL(&core_reg.H);
		break;
	case 0x15:
		ll_RL(&core_reg.L);
		break;
	case 0x16:
		temp = mem_read_u8(core_reg.HL);
		ll_RL(&temp);
		mem_write_u8(core_reg.HL, temp);
		break;
	case 0x17:
		ll_RL(&core_reg.A);
		break;
	}
}

// Rotate Right
static void op_RRC(uint8_t opcode)
{
	uint8_t temp;

	switch(opcode)
	{
	case 0x08:
		ll_RRC(&core_reg.B);
		break;
	case 0x09:
		ll_RRC(&core_reg.C);
		break;
	case 0x0A:
		ll_RRC(&core_reg.D);
		break;
	case 0x0B:
		ll_RRC(&core_reg.E);
		break;
	case 0x0C:
		ll_RRC(&core_reg.H);
		break;
	case 0x0D:
		ll_RRC(&core_reg.L);
		break;
	case 0x0E:
		temp = mem_read_u8(core_reg.HL);
		ll_RRC(&temp);
		mem_write_u8(core_reg.HL, temp);
		break;
	case 0x0F:
		ll_RRC(&core_reg.A);
		break;
	}
}

// Rotate Right  through Carry flag
static void op_RR(uint8_t opcode)
{
	uint8_t temp;

	switch(opcode)
	{
	case 0x18:
		ll_RR(&core_reg.B);
		break;
	case 0x19:
		ll_RR(&core_reg.C);
		break;
	case 0x1A:
		ll_RR(&core_reg.D);
		break;
	case 0x1B:
		ll_RR(&core_reg.E);
		break;
	case 0x1C:
		ll_RR(&core_reg.H);
		break;
	case 0x1D:
		ll_RR(&core_reg.L);
		break;
	case 0x1E:
		temp = mem_read_u8(core_reg.HL);
		ll_RR(&temp);
		mem_write_u8(core_reg.HL, temp);
		break;
	case 0x1F:
		ll_RR(&core_reg.A);
		break;
	}
}


struct opcode_t opcodeCbList[256] =
{
	{op_RLC,		2,	8,	true},		// 0x00
	{op_RLC,		2,	8,	true},		// 0x01
	{op_RLC,		2,	8,	true},		// 0x02
	{op_RLC,		2,	8,	true},		// 0x03
	{op_RLC,		2,	8,	true},		// 0x04
	{op_RLC,		2,	8,	true},		// 0x05
	{op_RLC,		2,	16,	true},		// 0x06
	{op_RLC,		2,	8,	true},		// 0x07
	{op_RRC,		2,	8,	true},		// 0x08
	{op_RRC,		2,	8,	true},		// 0x09
	{op_RRC,		2,	8,	true},		// 0x0A
	{op_RRC,		2,	8,	true},		// 0x0B
	{op_RRC,		2,	8,	true},		// 0x0C
	{op_RRC,		2,	8,	true},		// 0x0D
	{op_RRC,		2,	16,	true},		// 0x0E
	{op_RRC,		2,	8,	true},		// 0x0F
	{op_RL,			2,	8,	true},		// 0x10
	{op_RL,			2,	8,	true},		// 0x11
	{op_RL,			2,	8,	true},		// 0x12
	{op_RL,			2,	8,	true},		// 0x13
	{op_RL,			2,	8,	true},		// 0x14
	{op_RL,			2,	8,	true},		// 0x15
	{op_RL,			2,	16,	true},		// 0x16
	{op_RL,			2,	8,	true},		// 0x17
	{op_RR,			2,	8,	true},		// 0x18
	{op_RR,			2,	8,	true},		// 0x19
	{op_RR,			2,	8,	true},		// 0x1A
	{op_RR,			2,	8,	true},		// 0x1B
	{op_RR,			2,	8,	true},		// 0x1C
	{op_RR,			2,	8,	true},		// 0x1D
	{op_RR,			2,	16,	true},		// 0x1E
	{op_RR,			2,	8,	true},		// 0x1F
};
