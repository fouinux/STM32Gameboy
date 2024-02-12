/*
 * opcode.c
 *
 *  Created on: Jul 6, 2020
 *      Author: Guillaume Fouilleul
 */

#include "cpu.h"
#include "irq.h"
#include "mem.h"
#include "opcode.h"
#include "opcode_cb.h"
#include <stdio.h>

//////////////////////
//    8-bit Load    //
//////////////////////

// Macro: LD r1, r2
#define MACRO_LD_r1_r2(r1, r2) \
static uint8_t LD_##r1##_##r2(void) \
{ \
    cpu.reg.r1 = cpu.reg.r2; \
    return 1; \
}

MACRO_LD_r1_r2(B, B);    // LD B, B
MACRO_LD_r1_r2(B, C);    // LD B, C
MACRO_LD_r1_r2(B, D);    // LD B, D
MACRO_LD_r1_r2(B, E);    // LD B, E
MACRO_LD_r1_r2(B, H);    // LD B, H
MACRO_LD_r1_r2(B, L);    // LD B, L
MACRO_LD_r1_r2(B, A);    // LD B, A

MACRO_LD_r1_r2(C, B);    // LD C, B
MACRO_LD_r1_r2(C, C);    // LD C, C
MACRO_LD_r1_r2(C, D);    // LD C, D
MACRO_LD_r1_r2(C, E);    // LD C, E
MACRO_LD_r1_r2(C, H);    // LD C, H
MACRO_LD_r1_r2(C, L);    // LD C, L
MACRO_LD_r1_r2(C, A);    // LD C, A

MACRO_LD_r1_r2(D, B);    // LD D, B
MACRO_LD_r1_r2(D, C);    // LD D, C
MACRO_LD_r1_r2(D, D);    // LD D, D
MACRO_LD_r1_r2(D, E);    // LD D, E
MACRO_LD_r1_r2(D, H);    // LD D, H
MACRO_LD_r1_r2(D, L);    // LD D, L
MACRO_LD_r1_r2(D, A);    // LD D, A

MACRO_LD_r1_r2(E, B);    // LD E, B
MACRO_LD_r1_r2(E, C);    // LD E, C
MACRO_LD_r1_r2(E, D);    // LD E, D
MACRO_LD_r1_r2(E, E);    // LD E, E
MACRO_LD_r1_r2(E, H);    // LD E, H
MACRO_LD_r1_r2(E, L);    // LD E, L
MACRO_LD_r1_r2(E, A);    // LD E, A

MACRO_LD_r1_r2(H, B);    // LD H, B
MACRO_LD_r1_r2(H, C);    // LD H, C
MACRO_LD_r1_r2(H, D);    // LD H, D
MACRO_LD_r1_r2(H, E);    // LD H, E
MACRO_LD_r1_r2(H, H);    // LD H, H
MACRO_LD_r1_r2(H, L);    // LD H, L
MACRO_LD_r1_r2(H, A);    // LD H, A

MACRO_LD_r1_r2(L, B);    // LD L, B
MACRO_LD_r1_r2(L, C);    // LD L, C
MACRO_LD_r1_r2(L, D);    // LD L, D
MACRO_LD_r1_r2(L, E);    // LD L, E
MACRO_LD_r1_r2(L, H);    // LD L, H
MACRO_LD_r1_r2(L, L);    // LD L, L
MACRO_LD_r1_r2(L, A);    // LD L, A

MACRO_LD_r1_r2(A, B);    // LD A, B
MACRO_LD_r1_r2(A, C);    // LD A, C
MACRO_LD_r1_r2(A, D);    // LD A, D
MACRO_LD_r1_r2(A, E);    // LD A, E
MACRO_LD_r1_r2(A, H);    // LD A, H
MACRO_LD_r1_r2(A, L);    // LD A, L
MACRO_LD_r1_r2(A, A);    // LD A, A

#undef MACRO_LD_r1_r2

// Macro: LD r1, (HL)
#define MACRO_LD_r1_HL(r1) \
static uint8_t LD_##r1##_HL(void) \
{ \
    cpu.reg.r1 = mem_read_u8(cpu.reg.HL); \
    return 2; \
}

MACRO_LD_r1_HL(B);        // LD B, (HL)
MACRO_LD_r1_HL(C);        // LD C, (HL)
MACRO_LD_r1_HL(D);        // LD D, (HL)
MACRO_LD_r1_HL(E);        // LD E, (HL)
MACRO_LD_r1_HL(H);        // LD H, (HL)
MACRO_LD_r1_HL(L);        // LD L, (HL)
MACRO_LD_r1_HL(A);        // LD A, (HL)

#undef MACRO_LD_r1_HL

// Macro: LD (HL), r1
#define MACRO_LD_HL_r1(r1) \
static uint8_t LD_HL_##r1(void) \
{ \
    mem_write_u8(cpu.reg.HL, cpu.reg.r1); \
    return 2; \
}

MACRO_LD_HL_r1(B);        // LD (HL), B
MACRO_LD_HL_r1(C);        // LD (HL), C
MACRO_LD_HL_r1(D);        // LD (HL), D
MACRO_LD_HL_r1(E);        // LD (HL), E
MACRO_LD_HL_r1(H);        // LD (HL), H
MACRO_LD_HL_r1(L);        // LD (HL), L
MACRO_LD_HL_r1(A);        // LD (HL), A

#undef MACRO_LD_HL_r1

// Macro: LD (HL), r1
#define MACRO_LD_r1_d8(r1) \
static uint8_t LD_##r1##_d8(void) \
{ \
    cpu.reg.r1 = mem_read_u8(cpu.reg.PC + 1); \
    return 2; \
}

MACRO_LD_r1_d8(B);        // LD B, d8
MACRO_LD_r1_d8(C);        // LD C, d8
MACRO_LD_r1_d8(D);        // LD D, d8
MACRO_LD_r1_d8(E);        // LD E, d8
MACRO_LD_r1_d8(H);        // LD H, d8
MACRO_LD_r1_d8(L);        // LD L, d8
MACRO_LD_r1_d8(A);        // LD A, d8

#undef MACRO_LD_r1_d8

// LD (HL), d8
static uint8_t LD_HL_d8(void)
{
    mem_write_u8(cpu.reg.HL, mem_read_u8(cpu.reg.PC + 1));
    return 3;
}

// Macro: LD (r1), A
#define MACRO_LD_r1_A(r1) \
static uint8_t LD_##r1##_A(void) \
{ \
    mem_write_u8(cpu.reg.r1, cpu.reg.A); \
    return 2; \
}

MACRO_LD_r1_A(BC);        // LD (BC), A
MACRO_LD_r1_A(DE);        // LD (DE), A

#undef MACRO_LD_r1_A

// LD (HL+), A
static uint8_t LD_HLp_A(void)
{
    mem_write_u8(cpu.reg.HL++, cpu.reg.A);
    return 2;
}

// LD (HL-), A
static uint8_t LD_HLm_A(void)
{
    mem_write_u8(cpu.reg.HL--, cpu.reg.A);
    return 2;
}

// Macro: LD (r1), A
#define MACRO_LD_A_r1(r1) \
static uint8_t LD_A_##r1(void) \
{ \
    cpu.reg.A = mem_read_u8(cpu.reg.r1); \
    return 2; \
}

MACRO_LD_A_r1(BC);        // LD A, (BC)
MACRO_LD_A_r1(DE);        // LD A, (DE)

#undef MACRO_LD_A_r1

// LD A, (HL+)
static uint8_t LD_A_HLp(void)
{
    cpu.reg.A = mem_read_u8(cpu.reg.HL++);
    return 2;
}

// LD A, (HL-)
static uint8_t LD_A_HLm(void)
{
    cpu.reg.A = mem_read_u8(cpu.reg.HL--);
    return 2;
}

// LDH (a8), A
static uint8_t LDH_a8_A(void)
{
    uint8_t a8 = mem_read_u8(cpu.reg.PC + 1);
    mem_write_u8(0xFF00 + a8, cpu.reg.A);
    return 3;
}

// LDH A, (a8)
static uint8_t LDH_A_a8(void)
{
    uint8_t a8 = mem_read_u8(cpu.reg.PC + 1);
    cpu.reg.A = mem_read_u8(0xFF00 + a8);
    return 3;
}

// LD (C), A
static uint8_t LD_pC_A(void)
{
    mem_write_u8(0xFF00 + cpu.reg.C, cpu.reg.A);
    return 2;
}

// LD A, (C)
static uint8_t LD_A_pC(void)
{
    cpu.reg.A = mem_read_u8(0xFF00 + cpu.reg.C);
    return 2;
}

// LD (a16), A
static uint8_t LD_a16_A(void)
{
    uint16_t a16 = mem_read_u16(cpu.reg.PC + 1);
    mem_write_u8(a16, cpu.reg.A);
    return 4;
}

// LD A, (a16)
static uint8_t LD_A_a16(void)
{
    uint16_t a16 = mem_read_u16(cpu.reg.PC + 1);
    cpu.reg.A = mem_read_u8(a16);
    return 4;
}

//////////////////////
//   16-bit Load    //
//////////////////////

// Macro: LD r1, d16
#define MACRO_LD_r1_d16(r1) \
static uint8_t LD_##r1##_d16(void) \
{ \
    cpu.reg.r1 = mem_read_u16(cpu.reg.PC + 1); \
    return 3; \
}

MACRO_LD_r1_d16(BC);        // LD BC, d16
MACRO_LD_r1_d16(DE);        // LD DE, d16
MACRO_LD_r1_d16(HL);        // LD HL, d16
MACRO_LD_r1_d16(SP);        // LD SP, d16

#undef MACRO_LD_r1_d16

// LD (a16), SP
static uint8_t LD_a16_SP(void)
{
    uint16_t a16 = mem_read_u16(cpu.reg.PC + 1);

    mem_write_u16(a16, cpu.reg.SP);
    return 5;
}

// LD HL, SP+r8
static uint8_t LD_HL_SP_r8(void)
{
    int8_t r8 = mem_read_s8(cpu.reg.PC + 1);

    cpu.reg.HL = cpu.reg.SP + r8;
    cpu.reg.F = 0;
    if ((cpu.reg.SP & 0x0F) + (r8 & 0x0F) > 0x0F)
        cpu.reg.Flags.H = 1;

    if ((cpu.reg.SP & 0xFF) + ((uint16_t) r8 & 0xFF) > 0xFF)
        cpu.reg.Flags.C = 1;

    return 3;
}

// LD SP, HL
static uint8_t LD_SP_HL(void)
{
    cpu.reg.SP = cpu.reg.HL;
    return 2;
}

// Macro: PUSH r1
#define MACRO_PUSH_r1(r1) \
static uint8_t PUSH_##r1(void) \
{ \
    mem_write_u16(cpu.reg.SP - 2, cpu.reg.r1); \
    cpu.reg.SP -= 2; \
    return 4; \
}

MACRO_PUSH_r1(BC);     // PUSH BC
MACRO_PUSH_r1(DE);     // PUSH DE
MACRO_PUSH_r1(HL);     // PUSH HL
MACRO_PUSH_r1(AF);     // PUSH AF

#undef MACRO_PUSH_r1

// Macro: POP r1
#define MACRO_POP_r1(r1) \
static uint8_t POP_##r1(void) \
{ \
    cpu.reg.r1 = mem_read_u16(cpu.reg.SP); \
    cpu.reg.SP += 2; \
    return 3; \
}

MACRO_POP_r1(BC);     // POP BC
MACRO_POP_r1(DE);     // POP DE
MACRO_POP_r1(HL);     // POP HL

#undef MACRO_POP_r1

static uint8_t POP_AF(void)
{
    cpu.reg.AF = mem_read_u16(cpu.reg.SP) & 0xFFF0; // 4 LSB always zero
    cpu.reg.SP += 2;
    return 3;
}

//////////////////////
// Arithmetic 8-bit //
//////////////////////

// Macro: ADD A, r1
#define MACRO_ADD_A_r1(r1) \
static uint8_t ADD_A_##r1(void) \
{ \
    uint16_t t = cpu.reg.A + cpu.reg.r1; \
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);\
    cpu.reg.Flags.N = 0; \
    cpu.reg.Flags.H = ((cpu.reg.A & 0xF) + (cpu.reg.r1 & 0xF) > 0xF); \
    cpu.reg.Flags.C = (t > 0xFF); \
    cpu.reg.A = t & 0xFF; \
    return 1; \
}

MACRO_ADD_A_r1(B);    // ADD A, B
MACRO_ADD_A_r1(C);    // ADD A, C
MACRO_ADD_A_r1(D);    // ADD A, D
MACRO_ADD_A_r1(E);    // ADD A, E
MACRO_ADD_A_r1(H);    // ADD A, H
MACRO_ADD_A_r1(L);    // ADD A, L
MACRO_ADD_A_r1(A);    // ADD A, A

#undef MACRO_ADD_A_r1

// ADD A, (HL)
static uint8_t ADD_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    uint16_t t = cpu.reg.A + reg;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 0;
    cpu.reg.Flags.H = ((cpu.reg.A & 0xF) + (reg & 0xF) > 0xF);
    cpu.reg.Flags.C = (t > 0xFF);
    cpu.reg.A = t & 0xFF;
    return 2;
}

// ADD A, d8
static uint8_t ADD_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    uint16_t t = cpu.reg.A + d8;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 0;
    cpu.reg.Flags.H = ((cpu.reg.A & 0xF) + (d8 & 0xF) > 0xF);
    cpu.reg.Flags.C = (t > 0xFF);
    cpu.reg.A = t & 0xFF;
    return 2;
}

// Macro: ADC A, r1
#define MACRO_ADC_A_r1(r1) \
static uint8_t ADC_A_##r1(void) \
{ \
    uint16_t t = cpu.reg.A + cpu.reg.r1 + cpu.reg.Flags.C; \
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);\
    cpu.reg.Flags.N = 0; \
    cpu.reg.Flags.H = ((cpu.reg.A & 0xF) + (cpu.reg.r1 & 0xF) > 0xF); \
    cpu.reg.Flags.C = (t > 0xFF); \
    cpu.reg.A = t & 0xFF; \
    return 1; \
}

MACRO_ADC_A_r1(B);    // ADC A, B
MACRO_ADC_A_r1(C);    // ADC A, C
MACRO_ADC_A_r1(D);    // ADC A, D
MACRO_ADC_A_r1(E);    // ADC A, E
MACRO_ADC_A_r1(H);    // ADC A, H
MACRO_ADC_A_r1(L);    // ADC A, L
MACRO_ADC_A_r1(A);    // ADC A, A

#undef MACRO_ADC_A_r1

// ADC A, (HL)
static uint8_t ADC_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    uint16_t t = cpu.reg.A + reg +  cpu.reg.Flags.C;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 0;
    cpu.reg.Flags.H = ((cpu.reg.A & 0xF) + (reg & 0xF) > 0xF);
    cpu.reg.Flags.C = (t > 0xFF);
    cpu.reg.A = t & 0xFF;
    return 2;
}

// ADC A, d8
static uint8_t ADC_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    uint16_t t = cpu.reg.A + d8 +  cpu.reg.Flags.C;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 0;
    cpu.reg.Flags.H = ((cpu.reg.A & 0xF) + (d8 & 0xF) > 0xF);
    cpu.reg.Flags.C = (t > 0xFF);
    cpu.reg.A = t & 0xFF;
    return 2;
}

// Macro: SUB A, r1
#define MACRO_SUB_A_r1(r1) \
static uint8_t SUB_A_##r1(void) \
{ \
    int16_t t = cpu.reg.A - cpu.reg.r1; \
    cpu.reg.Flags.Z = ((t & 0xFF) == 0); \
    cpu.reg.Flags.N = 1; \
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) cpu.reg.r1 & 0xF) < 0); \
    cpu.reg.Flags.C = (t < 0); \
    cpu.reg.A = t & 0xFF; \
    return 1; \
}

MACRO_SUB_A_r1(B);    // SUB A, B
MACRO_SUB_A_r1(C);    // SUB A, C
MACRO_SUB_A_r1(D);    // SUB A, D
MACRO_SUB_A_r1(E);    // SUB A, E
MACRO_SUB_A_r1(H);    // SUB A, H
MACRO_SUB_A_r1(L);    // SUB A, L
MACRO_SUB_A_r1(A);    // SUB A, A

#undef MACRO_SUB_A_r1

// SUB A, (HL)
static uint8_t SUB_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    int16_t t = cpu.reg.A - reg;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 1;
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
    cpu.reg.Flags.C = (t < 0);
    cpu.reg.A = t & 0xFF;
    return 2;
}

// SUB A, d8
static uint8_t SUB_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    int16_t t = cpu.reg.A - d8;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 1;
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) d8 & 0xF) < 0);
    cpu.reg.Flags.C = (t < 0);
    cpu.reg.A = t & 0xFF;
    return 2;
}


// Macro: SBC A, r1
#define MACRO_SBC_A_r1(r1) \
static uint8_t SBC_A_##r1(void) \
{ \
    int16_t t = cpu.reg.A - cpu.reg.r1 - cpu.reg.Flags.C; \
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);\
    cpu.reg.Flags.N = 1; \
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) cpu.reg.r1 & 0xF) < 0); \
    cpu.reg.Flags.C = (t < 0); \
    cpu.reg.A = t & 0xFF; \
    return 1; \
}

MACRO_SBC_A_r1(B);    // SBC A, B
MACRO_SBC_A_r1(C);    // SBC A, C
MACRO_SBC_A_r1(D);    // SBC A, D
MACRO_SBC_A_r1(E);    // SBC A, E
MACRO_SBC_A_r1(H);    // SBC A, H
MACRO_SBC_A_r1(L);    // SBC A, L
MACRO_SBC_A_r1(A);    // SBC A, A

#undef MACRO_SBC_A_r1

// SBC A, (HL)
static uint8_t SBC_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    int16_t t = cpu.reg.A - reg - cpu.reg.Flags.C;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 1;
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
    cpu.reg.Flags.C = (t < 0);
    cpu.reg.A = t & 0xFF;
    return 2;
}

// SBC A, d8
static uint8_t SBC_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    int16_t t = cpu.reg.A - d8 - cpu.reg.Flags.C;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 1;
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) d8 & 0xF) < 0);
    cpu.reg.Flags.C = (t < 0);
    cpu.reg.A = t & 0xFF;
    return 2;
}

// Macro: AND A, r1
#define MACRO_AND_A_r1(r1) \
static uint8_t AND_A_##r1(void) \
{ \
    cpu.reg.A &= cpu.reg.r1; \
    cpu.reg.F = 0x20; /* N = 0, H = 1, C = 0 */ \
    cpu.reg.Flags.Z = (cpu.reg.A == 0);\
    return 1; \
}

MACRO_AND_A_r1(B);    // AND A, B
MACRO_AND_A_r1(C);    // AND A, C
MACRO_AND_A_r1(D);    // AND A, D
MACRO_AND_A_r1(E);    // AND A, E
MACRO_AND_A_r1(H);    // AND A, H
MACRO_AND_A_r1(L);    // AND A, L
MACRO_AND_A_r1(A);    // AND A, A

#undef MACRO_AND_A_r1

// AND A, (HL)
static uint8_t AND_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    cpu.reg.A &= reg;
    cpu.reg.F = 0x20; /* N = 0, H = 1, C = 0 */
    cpu.reg.Flags.Z = (cpu.reg.A == 0);
    return 2;
}

// AND A, d8
static uint8_t AND_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    cpu.reg.A &= d8;
    cpu.reg.F = 0x20; /* N = 0, H = 1, C = 0 */
    cpu.reg.Flags.Z = (cpu.reg.A == 0);
    return 2;
}


// Macro: XOR A, r1
#define MACRO_XOR_A_r1(r1) \
static uint8_t XOR_A_##r1(void) \
{ \
    cpu.reg.A ^= cpu.reg.r1; \
    cpu.reg.F = 0x00; /* N = 0, H = 0, C = 0 */ \
    cpu.reg.Flags.Z = (cpu.reg.A == 0);\
    return 1; \
}

MACRO_XOR_A_r1(B);    // XOR A, B
MACRO_XOR_A_r1(C);    // XOR A, C
MACRO_XOR_A_r1(D);    // XOR A, D
MACRO_XOR_A_r1(E);    // XOR A, E
MACRO_XOR_A_r1(H);    // XOR A, H
MACRO_XOR_A_r1(L);    // XOR A, L
MACRO_XOR_A_r1(A);    // XOR A, A

#undef MACRO_XOR_A_r1

// XOR A, (HL)
static uint8_t XOR_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    cpu.reg.A ^= reg;
    cpu.reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    cpu.reg.Flags.Z = (cpu.reg.A == 0);
    return 2;
}

// XOR A, d8
static uint8_t XOR_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    cpu.reg.A ^= d8;
    cpu.reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    cpu.reg.Flags.Z = (cpu.reg.A == 0);
    return 2;
}

// Macro: OR A, r1
#define MACRO_OR_A_r1(r1) \
static uint8_t OR_A_##r1(void) \
{ \
    cpu.reg.A |= cpu.reg.r1; \
    cpu.reg.F = 0x00; /* N = 0, H = 0, C = 0 */ \
    cpu.reg.Flags.Z = (cpu.reg.A == 0); \
    return 1; \
}

MACRO_OR_A_r1(B);    // OR A, B
MACRO_OR_A_r1(C);    // OR A, C
MACRO_OR_A_r1(D);    // OR A, D
MACRO_OR_A_r1(E);    // OR A, E
MACRO_OR_A_r1(H);    // OR A, H
MACRO_OR_A_r1(L);    // OR A, L
MACRO_OR_A_r1(A);    // OR A, A

#undef MACRO_OR_A_r1

// OR A, (HL)
static uint8_t OR_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    cpu.reg.A |= reg;
    cpu.reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    cpu.reg.Flags.Z = (cpu.reg.A == 0);
    return 2;
}

// OR A, d8
static uint8_t OR_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    cpu.reg.A |= d8;
    cpu.reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    cpu.reg.Flags.Z = (cpu.reg.A == 0);
    return 2;
}

// Macro: CP A, r1
#define MACRO_CP_A_r1(r1) \
static uint8_t CP_A_##r1(void) \
{ \
    int16_t t = cpu.reg.A - cpu.reg.r1; \
    cpu.reg.Flags.Z = ((t & 0xFF) == 0); \
    cpu.reg.Flags.N = 1; \
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) cpu.reg.r1 & 0xF) < 0); \
    cpu.reg.Flags.C = (t < 0); \
    return 1; \
}

MACRO_CP_A_r1(B);    // CP A, B
MACRO_CP_A_r1(C);    // CP A, C
MACRO_CP_A_r1(D);    // CP A, D
MACRO_CP_A_r1(E);    // CP A, E
MACRO_CP_A_r1(H);    // CP A, H
MACRO_CP_A_r1(L);    // CP A, L
MACRO_CP_A_r1(A);    // CP A, A

#undef MACRO_CP_A_r1

// CP A, (HL)
static uint8_t CP_A_HL(void)
{
    uint8_t reg = mem_read_u8(cpu.reg.HL);
    int16_t t = cpu.reg.A - reg;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 1;
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
    cpu.reg.Flags.C = (t < 0);
    return 2;
}

// CP A, d8
static uint8_t CP_A_d8(void)
{
    uint8_t d8 = mem_read_u8(cpu.reg.PC + 1);
    int16_t t = cpu.reg.A - d8;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.N = 1;
    cpu.reg.Flags.H = (((int8_t) cpu.reg.A & 0xF) - ((int8_t) d8 & 0xF) < 0);
    cpu.reg.Flags.C = (t < 0);
    return 2;
}

// Macro: INC r1
#define MACRO_INC_r1(r1) \
static uint8_t INC_##r1(void) \
{ \
    cpu.reg.r1++; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0x00); \
    cpu.reg.Flags.N = 0; \
    cpu.reg.Flags.H = ((cpu.reg.r1 & 0x0F) == 0x00); \
    return 1; \
}

MACRO_INC_r1(B);    // INC B
MACRO_INC_r1(C);    // INC C
MACRO_INC_r1(D);    // INC D
MACRO_INC_r1(E);    // INC E
MACRO_INC_r1(H);    // INC H
MACRO_INC_r1(L);    // INC L
MACRO_INC_r1(A);    // INC A

#undef MACRO_INC_r1

// INC (HL)
static uint8_t INC_pHL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    t++;
    cpu.reg.Flags.Z = (t == 0x00);
    cpu.reg.Flags.N = 0;
    cpu.reg.Flags.H = ((t & 0x0F) == 0x00);

    mem_write_u8(cpu.reg.HL, t);
    return 3;
}

// Macro: DEC r1
#define MACRO_DEC_r1(r1) \
static uint8_t DEC_##r1(void) \
{ \
    cpu.reg.r1--; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0x00); \
    cpu.reg.Flags.N = 1; \
    cpu.reg.Flags.H = ((cpu.reg.r1 & 0x0F) == 0x0F); \
    return 1; \
}

MACRO_DEC_r1(B);    // DEC B
MACRO_DEC_r1(C);    // DEC C
MACRO_DEC_r1(D);    // DEC D
MACRO_DEC_r1(E);    // DEC E
MACRO_DEC_r1(H);    // DEC H
MACRO_DEC_r1(L);    // DEC L
MACRO_DEC_r1(A);    // DEC A

#undef MACRO_DEC_r1

// DEC (HL)
static uint8_t DEC_pHL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    t--;
    cpu.reg.Flags.Z = (t == 0x00);
    cpu.reg.Flags.N = 1;
    cpu.reg.Flags.H = ((t & 0x0F) == 0x0F);

    mem_write_u8(cpu.reg.HL, t);
    return 3;
}

//////////////////////
// Arithmetic 16bit //
//////////////////////

// Macro: INC r1
#define MACRO_INC_r1(r1) \
static uint8_t INC_##r1(void) \
{ \
    cpu.reg.r1++; \
    return 2; \
}

MACRO_INC_r1(BC);    // INC BC
MACRO_INC_r1(DE);    // INC DE
MACRO_INC_r1(HL);    // INC HL
MACRO_INC_r1(SP);    // INC SP

#undef MACRO_INC_r1

// Macro: DEC r1
#define MACRO_DEC_r1(r1) \
static uint8_t DEC_##r1(void) \
{ \
    cpu.reg.r1--; \
    return 2; \
}

MACRO_DEC_r1(BC);    // DEC BC
MACRO_DEC_r1(DE);    // DEC DE
MACRO_DEC_r1(HL);    // DEC HL
MACRO_DEC_r1(SP);    // DEC SP

#undef MACRO_DEC_r1

// Macro: ADD HL r1
#define MACRO_ADD_HL_r1(r1) \
static uint8_t ADD_HL_##r1(void) \
{ \
    uint32_t result = (uint32_t) cpu.reg.HL + (uint32_t) cpu.reg.r1; \
 \
    cpu.reg.Flags.N = 0; \
    cpu.reg.Flags.C = (result > 0xFFFF); \
 \
    if ((cpu.reg.HL & 0xFFF) + (cpu.reg.r1 & 0xFFF) > 0xFFF) \
        cpu.reg.Flags.H = 1; \
    else \
        cpu.reg.Flags.H = 0; \
 \
    cpu.reg.HL = result & 0xFFFF; \
    return 2; \
}

MACRO_ADD_HL_r1(BC);    // ADD HL, BC
MACRO_ADD_HL_r1(DE);    // ADD HL, DE
MACRO_ADD_HL_r1(HL);    // ADD HL, HL
MACRO_ADD_HL_r1(SP);    // ADD HL, SP

#undef MACRO_ADD_HL_r1

// ADD SP, r8
static uint8_t ADD_SP_r8(void)
{
    int8_t r8 = mem_read_s8(cpu.reg.PC + 1);

    cpu.reg.F = 0;
    if ((cpu.reg.SP & 0x0F) + (r8 & 0x0F) > 0x0F)
        cpu.reg.Flags.H = 1;

    if ((cpu.reg.SP & 0xFF) + ((uint16_t) r8 & 0xFF) > 0xFF)
        cpu.reg.Flags.C = 1;

    cpu.reg.SP += r8;
    return 4;
}

//////////////////////
//       Misc       //
//////////////////////

// Decimal adjust A register - DAA
static uint8_t DAA(void)
{
    if (cpu.reg.Flags.N == 0)
    {
        if (cpu.reg.Flags.C || cpu.reg.A > 0x99)
        {
            cpu.reg.A += 0x60;
            cpu.reg.Flags.C = 1;
        }

        if (cpu.reg.Flags.H || (cpu.reg.A & 0x0F) > 0x09)
            cpu.reg.A += 0x06;
    }
    else
    {
        if (cpu.reg.Flags.C)
            cpu.reg.A -= 0x60;

        if (cpu.reg.Flags.H)
            cpu.reg.A -= 0x06;
    }

    cpu.reg.Flags.Z = (cpu.reg.A == 0);
    cpu.reg.Flags.H = 0;
    return 1;
}

// Complement A register - CPL
static uint8_t CPL(void)
{
    cpu.reg.F |= 0x60; // Set N & H
    cpu.reg.A ^= 0xFF;
    return 1;
}

// Complement Carry Flag - CCF
static uint8_t CCF(void)
{
    cpu.reg.F &= 0x90; // Reset N & H
    cpu.reg.Flags.C = !cpu.reg.Flags.C;
    return 1;
}

// Set Carry Flag - SCF
static uint8_t SCF(void)
{
    cpu.reg.F &= 0x90; // Reset N & H
    cpu.reg.Flags.C = 1;
    return 1;
}

// NOP
static uint8_t NOP(void)
{
    // Do nothing for one cycle
    return 1;
}

// HALT
static uint8_t HALT(void)
{
    // TODO Halt
    return 1;
}

// STOP
static uint8_t STOP(void)
{
    // TODO Stop
    return 1;
}

// DI
static uint8_t DI(void)
{
    // Disable IRQ
    irq.ime = false;
    return 1;
}

// EI
static uint8_t EI(void)
{
    // Enable IRQ
    irq.ime = true;
    return 1;
}

// PREFIX CB
static uint8_t PREFIX_CB(void)
{
    cpu.prefix_cb = true;
    return 1;
}

//////////////////////
// Rotates & Shifts //
//////////////////////

// Rotate A Left
static uint8_t RLCA(void)
{
    cpu.reg.A = (cpu.reg.A << 1) | (cpu.reg.A >> 7);
    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = cpu.reg.A & 0x01;
    return 1;
}

// Rotate A Left through Carry flag
static uint8_t RLA(void)
{
    uint16_t t = (cpu.reg.A << 1) | cpu.reg.Flags.C;
    cpu.reg.A = t & 0xFF;
    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = (t > 0xFF);
    return 1;
}

// Rotate A Right
static uint8_t RRCA(void)
{
    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = cpu.reg.A & 0x01;
    cpu.reg.A = (cpu.reg.A >> 1) | (cpu.reg.A << 7);
    return 1;
}

// Rotate A Right through Carry flag
static uint8_t RRA(void)
{
    uint8_t t = (cpu.reg.A >> 1) | (cpu.reg.Flags.C << 7);
    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = cpu.reg.A & 0x01;
    cpu.reg.A = t;
    return 1;
}

//////////////////////
//     Jumps        //
//////////////////////

// JP a16
static uint8_t JP_a16(void)
{
    cpu.reg.PC = mem_read_u16(cpu.reg.PC + 1);
    return 4;
}

// JP HL
static uint8_t JP_HL(void)
{
    cpu.reg.PC = cpu.reg.HL;
    return 1;
}

// JR r8
static uint8_t JR_r8(void)
{
    int8_t r8 = mem_read_s8(cpu.reg.PC + 1);
    cpu.reg.PC += 2 + r8;
    return 3;
}

// Macro: JP COND, a16
#define MACRO_JP_COND_a16(name, bit, state) \
static uint8_t JP_##name##_a16(void) \
{ \
    uint16_t a16 = mem_read_u16(cpu.reg.PC + 1); \
    if (cpu.reg.Flags.bit == state) \
    { \
        cpu.reg.PC = a16; /* Jump */ \
        return 4; \
    } \
    else \
    { \
        cpu.reg.PC += 3; /* Next opcode */ \
        return 3; \
    } \
}

MACRO_JP_COND_a16(NZ, Z, 0);    // JP NZ, a16
MACRO_JP_COND_a16(Z, Z, 1);     // JP Z, a16
MACRO_JP_COND_a16(NC, C, 0);    // JP NC, a16
MACRO_JP_COND_a16(C, C, 1);     // JP C, a16

#undef MACRO_JP_COND_a16

// Macro: JR COND r8
#define MACRO_JR_COND_r8(name, bit, state) \
static uint8_t JR_##name##_r8(void) \
{ \
    int8_t r8 = mem_read_s8(cpu.reg.PC + 1); \
    cpu.reg.PC += 2; \
    if (cpu.reg.Flags.bit == state) \
    { \
        cpu.reg.PC += r8; /* Relative jump */ \
        return 3; \
    } \
    else \
    { \
        /* Next opcode */ \
        return 2; \
    } \
}

MACRO_JR_COND_r8(NZ, Z, 0);     // JR NZ, r8
MACRO_JR_COND_r8(Z, Z, 1);      // JR Z, r8
MACRO_JR_COND_r8(NC, C, 0);     // JR NC, r8
MACRO_JR_COND_r8(C, C, 1);      // JR C, r8

#undef MACRO_JR_COND_r8

//////////////////////
//      Calls       //
//////////////////////

// Macro: CALL COND, a16
#define MACRO_CALL_COND_a16(name, bit, state) \
static uint8_t CALL_##name##_a16(void) \
{ \
    uint16_t a16 = mem_read_u16(cpu.reg.PC + 1); \
    if (cpu.reg.Flags.bit == state) \
    { \
        mem_write_u16(cpu.reg.SP - 2, cpu.reg.PC); /* Save PC */ \
        cpu.reg.PC = a16; /* Jump */ \
        cpu.reg.SP -= 2; \
        return 6; \
    } \
    else \
    { \
        cpu.reg.PC += 3; /* Next opcode */ \
        return 3; \
    } \
}

MACRO_CALL_COND_a16(NZ, Z, 0);     // CALL NZ, a16
MACRO_CALL_COND_a16(Z, Z, 1);      // CALL Z, a16
MACRO_CALL_COND_a16(NC, C, 0);     // CALL NC, a16
MACRO_CALL_COND_a16(C, C, 1);      // CALL C, a16

#undef MACRO_CALL_COND_a16

// CALL a16
static uint8_t CALL_a16(void) \
{
    uint16_t a16 = mem_read_u16(cpu.reg.PC + 1);
    mem_write_u16(cpu.reg.SP - 2, cpu.reg.PC + 3);
    cpu.reg.SP -= 2;
    cpu.reg.PC = a16;
    return 6;
}

//////////////////////
//     Restart      //
//////////////////////

// Macro: RST nnH
#define MACRO_RST_nnH(nn) \
static uint8_t RST_##nn##H(void) \
{ \
    mem_write_u16(cpu.reg.SP - 2, cpu.reg.PC + 1); \
    cpu.reg.SP -= 2; \
    cpu.reg.PC = 0x##nn; \
    return 4; \
}

MACRO_RST_nnH(00);    // RST 00H
MACRO_RST_nnH(08);    // RST 08H
MACRO_RST_nnH(10);    // RST 10H
MACRO_RST_nnH(18);    // RST 18H
MACRO_RST_nnH(20);    // RST 20H
MACRO_RST_nnH(28);    // RST 28H
MACRO_RST_nnH(30);    // RST 30H
MACRO_RST_nnH(38);    // RST 38H

#undef MACRO_RST_nnH


//////////////////////
//      Return      //
//////////////////////

// RET
static uint8_t RET(void)
{
    cpu.reg.PC = mem_read_u16(cpu.reg.SP); /* Jump to SP */
    cpu.reg.SP += 2;
    return 4;
}

// Macro: RET COND
#define MACRO_RET_COND(name, bit, state) \
static uint8_t RET_##name(void) \
{ \
    if (cpu.reg.Flags.bit == state) \
    { \
        cpu.reg.PC = mem_read_u16(cpu.reg.SP); /* Jump to SP */ \
        cpu.reg.SP += 2; \
        return 5; \
    } \
    else \
    { \
        cpu.reg.PC += 1; /* Next opcode */ \
        return 2; \
    } \
}

MACRO_RET_COND(NZ, Z, 0);     // RET NZ
MACRO_RET_COND(Z, Z, 1);      // RET Z
MACRO_RET_COND(NC, C, 0);     // RET NC
MACRO_RET_COND(C, C, 1);      // RET C

#undef MACRO_JR_COND_r8

// RETI
static uint8_t RETI(void)
{
    // Enable IRQ
    irq.ime = true;
    cpu.reg.PC = mem_read_u16(cpu.reg.SP);
    cpu.reg.SP += 2;
    return 4;
}

struct opcode_t opcodeList[256] =
{
        {NOP,           "NOP",          1,      true},      // 0x00
        {LD_BC_d16,     "LD BC,d16",    3,      true},      // 0x01
        {LD_BC_A,       "LD (BC),A",    1,      true},      // 0x02
        {INC_BC,        "INC BC",       1,      true},      // 0x03
        {INC_B,         "INC B",        1,      true},      // 0x04
        {DEC_B,         "DEC B",        1,      true},      // 0x05
        {LD_B_d8,       "LD B,d8",      2,      true},      // 0x06
        {RLCA,          "RLCA",         1,      true},      // 0x07
        {LD_a16_SP,     "LD (a16),SP",  3,      true},      // 0x08
        {ADD_HL_BC,     "ADD HL,BC",    1,      true},      // 0x09
        {LD_A_BC,       "LD A,(BC)",    1,      true},      // 0x0A
        {DEC_BC,        "DEC BC",       1,      true},      // 0x0B
        {INC_C,         "INC C",        1,      true},      // 0x0C
        {DEC_C,         "DEC C",        1,      true},      // 0x0D
        {LD_C_d8,       "LD C,d8",      2,      true},      // 0x0E
        {RRCA,          "RRCA",         1,      true},      // 0x0F
        {STOP,          "STOP 0",       2,      true},      // 0x10
        {LD_DE_d16,     "LD DE,d16",    3,      true},      // 0x11
        {LD_DE_A,       "LD (DE),A",    1,      true},      // 0x12
        {INC_DE,        "INC DE",       1,      true},      // 0x13
        {INC_D,         "INC D",        1,      true},      // 0x14
        {DEC_D,         "DEC D",        1,      true},      // 0x15
        {LD_D_d8,       "LD D,d8",      2,      true},      // 0x16
        {RLA,           "RLA",          1,      true},      // 0x17
        {JR_r8,         "JR r8",        2,      false},     // 0x18
        {ADD_HL_DE,     "ADD HL,DE",    1,      true},      // 0x19
        {LD_A_DE,       "LD A,(DE)",    1,      true},      // 0x1A
        {DEC_DE,        "DEC DE",       1,      true},      // 0x1B
        {INC_E,         "INC E",        1,      true},      // 0x1C
        {DEC_E,         "DEC E",        1,      true},      // 0x1D
        {LD_E_d8,       "LD E,d8",      2,      true},      // 0x1E
        {RRA,           "RRA",          1,      true},      // 0x1F
        {JR_NZ_r8,      "JR NZ,r8",     2,      false},     // 0x20
        {LD_HL_d16,     "LD HL,d16",    3,      true},      // 0x21
        {LD_HLp_A,      "LD (HL+),A",   1,      true},      // 0x22
        {INC_HL,        "INC HL",       1,      true},      // 0x23
        {INC_H,         "INC H",        1,      true},      // 0x24
        {DEC_H,         "DEC H",        1,      true},      // 0x25
        {LD_H_d8,       "LD H,d8",      2,      true},      // 0x26
        {DAA,           "DAA",          1,      true},      // 0x27
        {JR_Z_r8,       "JR Z,r8",      2,      false},     // 0x28
        {ADD_HL_HL,     "ADD HL,HL",    1,      true},      // 0x29
        {LD_A_HLp,      "LD A,(HL+)",   1,      true},      // 0x2A
        {DEC_HL,        "DEC HL",       1,      true},      // 0x2B
        {INC_L,         "INC L",        1,      true},      // 0x2C
        {DEC_L,         "DEC L",        1,      true},      // 0x2D
        {LD_L_d8,       "LD L,d8",      2,      true},      // 0x2E
        {CPL,           "CPL",          1,      true},      // 0x2F
        {JR_NC_r8,      "JR NC,r8",     2,      false},     // 0x30
        {LD_SP_d16,     "LD SP,d16",    3,      true},      // 0x31
        {LD_HLm_A,      "LD (HL-),A",   1,      true},      // 0x32
        {INC_SP,        "INC SP",       1,      true},      // 0x33
        {INC_pHL,       "INC (HL)",     1,      true},      // 0x34
        {DEC_pHL,       "DEC (HL)",     1,      true},      // 0x35
        {LD_HL_d8,      "LD (HL),d8",   2,      true},      // 0x36
        {SCF,           "SCF",          1,      true},      // 0x37
        {JR_C_r8,       "JR C,r8",      2,      false},     // 0x38
        {ADD_HL_SP,     "ADD HL,SP",    1,      true},      // 0x39
        {LD_A_HLm,      "LD A,(HL-)",   1,      true},      // 0x3A
        {DEC_SP,        "DEC SP",       1,      true},      // 0x3B
        {INC_A,         "INC A",        1,      true},      // 0x3C
        {DEC_A,         "DEC A",        1,      true},      // 0x3D
        {LD_A_d8,       "LD A,d8",      2,      true},      // 0x3E
        {CCF,           "CCF",          1,      true},      // 0x3F
        {LD_B_B,        "LD B,B",       1,      true},      // 0x40
        {LD_B_C,        "LD B,C",       1,      true},      // 0x41
        {LD_B_D,        "LD B,D",       1,      true},      // 0x42
        {LD_B_E,        "LD B,E",       1,      true},      // 0x43
        {LD_B_H,        "LD B,H",       1,      true},      // 0x44
        {LD_B_L,        "LD B,L",       1,      true},      // 0x45
        {LD_B_HL,       "LD B,(HL)",    1,      true},      // 0x46
        {LD_B_A,        "LD B,A",       1,      true},      // 0x47
        {LD_C_B,        "LD C,B",       1,      true},      // 0x48
        {LD_C_C,        "LD C,C",       1,      true},      // 0x49
        {LD_C_D,        "LD C,D",       1,      true},      // 0x4A
        {LD_C_E,        "LD C,E",       1,      true},      // 0x4B
        {LD_C_H,        "LD C,H",       1,      true},      // 0x4C
        {LD_C_L,        "LD C,L",       1,      true},      // 0x4D
        {LD_C_HL,       "LD C,(HL)",    1,      true},      // 0x4E
        {LD_C_A,        "LD C,A",       1,      true},      // 0x4F
        {LD_D_B,        "LD D,B",       1,      true},      // 0x50
        {LD_D_C,        "LD D,C",       1,      true},      // 0x51
        {LD_D_D,        "LD D,D",       1,      true},      // 0x52
        {LD_D_E,        "LD D,E",       1,      true},      // 0x53
        {LD_D_H,        "LD D,H",       1,      true},      // 0x54
        {LD_D_L,        "LD D,L",       1,      true},      // 0x55
        {LD_D_HL,       "LD D,(HL)",    1,      true},      // 0x56
        {LD_D_A,        "LD D,A",       1,      true},      // 0x57
        {LD_E_B,        "LD E,B",       1,      true},      // 0x58
        {LD_E_C,        "LD E,C",       1,      true},      // 0x59
        {LD_E_D,        "LD E,D",       1,      true},      // 0x5A
        {LD_E_E,        "LD E,E",       1,      true},      // 0x5B
        {LD_E_H,        "LD E,H",       1,      true},      // 0x5C
        {LD_E_L,        "LD E,L",       1,      true},      // 0x5D
        {LD_E_HL,       "LD E,(HL)",    1,      true},      // 0x5E
        {LD_E_A,        "LD E,A",       1,      true},      // 0x5F
        {LD_H_B,        "LD H,B",       1,      true},      // 0x60
        {LD_H_C,        "LD H,C",       1,      true},      // 0x61
        {LD_H_D,        "LD H,D",       1,      true},      // 0x62
        {LD_H_E,        "LD H,E",       1,      true},      // 0x63
        {LD_H_H,        "LD H,H",       1,      true},      // 0x64
        {LD_H_L,        "LD H,L",       1,      true},      // 0x65
        {LD_H_HL,       "LD H,(HL)",    1,      true},      // 0x66
        {LD_H_A,        "LD H,A",       1,      true},      // 0x67
        {LD_L_B,        "LD L,B",       1,      true},      // 0x68
        {LD_L_C,        "LD L,C",       1,      true},      // 0x69
        {LD_L_D,        "LD L,D",       1,      true},      // 0x6A
        {LD_L_E,        "LD L,E",       1,      true},      // 0x6B
        {LD_L_H,        "LD L,H",       1,      true},      // 0x6C
        {LD_L_L,        "LD L,L",       1,      true},      // 0x6D
        {LD_L_HL,       "LD L,(HL)",    1,      true},      // 0x6E
        {LD_L_A,        "LD L,A",       1,      true},      // 0x6F
        {LD_HL_B,       "LD (HL),B",    1,      true},      // 0x70
        {LD_HL_C,       "LD (HL),C",    1,      true},      // 0x71
        {LD_HL_D,       "LD (HL),D",    1,      true},      // 0x72
        {LD_HL_E,       "LD (HL),E",    1,      true},      // 0x73
        {LD_HL_H,       "LD (HL),H",    1,      true},      // 0x74
        {LD_HL_L,       "LD (HL),L",    1,      true},      // 0x75
        {HALT,          "HALT",         1,      true},      // 0x76
        {LD_HL_A,       "LD (HL),A",    1,      true},      // 0x77
        {LD_A_B,        "LD A,B",       1,      true},      // 0x78
        {LD_A_C,        "LD A,C",       1,      true},      // 0x79
        {LD_A_D,        "LD A,D",       1,      true},      // 0x7A
        {LD_A_E,        "LD A,E",       1,      true},      // 0x7B
        {LD_A_H,        "LD A,H",       1,      true},      // 0x7C
        {LD_A_L,        "LD A,L",       1,      true},      // 0x7D
        {LD_A_HL,       "LD A,(HL)",    1,      true},      // 0x7E
        {LD_A_A,        "LD A,A",       1,      true},      // 0x7F
        {ADD_A_B,       "ADD A,B",      1,      true},      // 0x80
        {ADD_A_C,       "ADD A,C",      1,      true},      // 0x81
        {ADD_A_D,       "ADD A,D",      1,      true},      // 0x82
        {ADD_A_E,       "ADD A,E",      1,      true},      // 0x83
        {ADD_A_H,       "ADD A,H",      1,      true},      // 0x84
        {ADD_A_L,       "ADD A,L",      1,      true},      // 0x85
        {ADD_A_HL,      "ADD A,(HL)",   1,      true},      // 0x86
        {ADD_A_A,       "ADD A,A",      1,      true},      // 0x87
        {ADC_A_B,       "ADC A,B",      1,      true},      // 0x88
        {ADC_A_C,       "ADC A,C",      1,      true},      // 0x89
        {ADC_A_D,       "ADC A,D",      1,      true},      // 0x8A
        {ADC_A_E,       "ADC A,E",      1,      true},      // 0x8B
        {ADC_A_H,       "ADC A,H",      1,      true},      // 0x8C
        {ADC_A_L,       "ADC A,L",      1,      true},      // 0x8D
        {ADC_A_HL,      "ADC A,(HL)",   1,      true},      // 0x8E
        {ADC_A_A,       "ADC A,A",      1,      true},      // 0x8F
        {SUB_A_B,       "SUB B",        1,      true},      // 0x90
        {SUB_A_C,       "SUB C",        1,      true},      // 0x91
        {SUB_A_D,       "SUB D",        1,      true},      // 0x92
        {SUB_A_E,       "SUB E",        1,      true},      // 0x93
        {SUB_A_H,       "SUB H",        1,      true},      // 0x94
        {SUB_A_L,       "SUB L",        1,      true},      // 0x95
        {SUB_A_HL,      "SUB (HL)",     1,      true},      // 0x96
        {SUB_A_A,       "SUB A",        1,      true},      // 0x97
        {SBC_A_B,       "SBC A,B",      1,      true},      // 0x98
        {SBC_A_C,       "SBC A,C",      1,      true},      // 0x99
        {SBC_A_D,       "SBC A,D",      1,      true},      // 0x9A
        {SBC_A_E,       "SBC A,E",      1,      true},      // 0x9B
        {SBC_A_H,       "SBC A,H",      1,      true},      // 0x9C
        {SBC_A_L,       "SBC A,L",      1,      true},      // 0x9D
        {SBC_A_HL,      "SBC A,(HL)",   1,      true},      // 0x9E
        {SBC_A_A,       "SBC A,A",      1,      true},      // 0x9F
        {AND_A_B,       "AND B",        1,      true},      // 0xA0
        {AND_A_C,       "AND C",        1,      true},      // 0xA1
        {AND_A_D,       "AND D",        1,      true},      // 0xA2
        {AND_A_E,       "AND E",        1,      true},      // 0xA3
        {AND_A_H,       "AND H",        1,      true},      // 0xA4
        {AND_A_L,       "AND L",        1,      true},      // 0xA5
        {AND_A_HL,      "AND (HL)",     1,      true},      // 0xA6
        {AND_A_A,       "AND A",        1,      true},      // 0xA7
        {XOR_A_B,       "XOR B",        1,      true},      // 0xA8
        {XOR_A_C,       "XOR C",        1,      true},      // 0xA9
        {XOR_A_D,       "XOR D",        1,      true},      // 0xAA
        {XOR_A_E,       "XOR E",        1,      true},      // 0xAB
        {XOR_A_H,       "XOR H",        1,      true},      // 0xAC
        {XOR_A_L,       "XOR L",        1,      true},      // 0xAD
        {XOR_A_HL,      "XOR (HL)",     1,      true},      // 0xAE
        {XOR_A_A,       "XOR A",        1,      true},      // 0xAF
        {OR_A_B,        "OR B",         1,      true},      // 0xB0
        {OR_A_C,        "OR C",         1,      true},      // 0xB1
        {OR_A_D,        "OR D",         1,      true},      // 0xB2
        {OR_A_E,        "OR E",         1,      true},      // 0xB3
        {OR_A_H,        "OR H",         1,      true},      // 0xB4
        {OR_A_L,        "OR L",         1,      true},      // 0xB5
        {OR_A_HL,       "OR (HL)",      1,      true},      // 0xB6
        {OR_A_A,        "OR A",         1,      true},      // 0xB7
        {CP_A_B,        "CP B",         1,      true},      // 0xB8
        {CP_A_C,        "CP C",         1,      true},      // 0xB9
        {CP_A_D,        "CP D",         1,      true},      // 0xBA
        {CP_A_E,        "CP E",         1,      true},      // 0xBB
        {CP_A_H,        "CP H",         1,      true},      // 0xBC
        {CP_A_L,        "CP L",         1,      true},      // 0xBD
        {CP_A_HL,       "CP (HL)",      1,      true},      // 0xBE
        {CP_A_A,        "CP A",         1,      true},      // 0xBF
        {RET_NZ,        "RET NZ",       1,      false},     // 0xC0
        {POP_BC,        "POP BC",       1,      true},      // 0xC1
        {JP_NZ_a16,     "JP NZ,a16",    3,      false},     // 0xC2
        {JP_a16,        "JP a16",       3,      false},     // 0xC3
        {CALL_NZ_a16,   "CALL NZ,a16",  3,      false},     // 0xC4
        {PUSH_BC,       "PUSH BC",      1,      true},      // 0xC5
        {ADD_A_d8,      "ADD A,d8",     2,      true},      // 0xC6
        {RST_00H,       "RST 00H",      1,      false},     // 0xC7
        {RET_Z,         "RET Z",        1,      false},     // 0xC8
        {RET,           "RET",          1,      false},     // 0xC9
        {JP_Z_a16,      "JP Z,a16",     3,      false},     // 0xCA
        {PREFIX_CB,     "PREFIX CB",    1,      true},      // 0xCB
        {CALL_Z_a16,    "CALL Z,a16",   3,      false},     // 0xCC
        {CALL_a16,      "CALL a16",     3,      false},     // 0xCD
        {ADC_A_d8,      "ADC A,d8",     2,      true},      // 0xCE
        {RST_08H,       "RST 08H",      1,      false},     // 0xCF
        {RET_NC,        "RET NC",       1,      false},     // 0xD0
        {POP_DE,        "POP DE",       1,      true},      // 0xD1
        {JP_NC_a16,     "JP NC,a16",    3,      false},     // 0xD2
        {NULL,          "",             0,      false},     // 0xD3
        {CALL_NC_a16,   "CALL NC,a16",  3,      false},     // 0xD4
        {PUSH_DE,       "PUSH DE",      1,      true},      // 0xD5
        {SUB_A_d8,      "SUB A,d8",     2,      true},      // 0xD6
        {RST_10H,       "RST 10H",      1,      false},     // 0xD7
        {RET_C,         "RET C",        1,      false},     // 0xD8
        {RETI,          "RETI",         1,      false},     // 0xD9
        {JP_C_a16,      "JP C,a16",     3,      false},     // 0xDA
        {NULL,          "",             0,      false},     // 0xDB
        {CALL_C_a16,    "CALL C,a16",   3,      false},     // 0xDC
        {NULL,          "",             0,      false},     // 0xDD
        {SBC_A_d8,      "SBC A,d8",     2,      true},      // 0xDE
        {RST_18H,       "RST 18H",      1,      false},     // 0xDF
        {LDH_a8_A,      "LDH (a8),A",   2,      true},      // 0xE0
        {POP_HL,        "POP HL",       1,      true},      // 0xE1
        {LD_pC_A,       "LD (C),A",     1,      true},      // 0xE2
        {NULL,          "",             0,      false},     // 0xE3
        {NULL,          "",             0,      false},     // 0xE4
        {PUSH_HL,       "PUSH HL",      1,      true},      // 0xE5
        {AND_A_d8,      "AND A,d8",     2,      true},      // 0xE6
        {RST_20H,       "RST 20H",      1,      false},     // 0xE7
        {ADD_SP_r8,     "ADD SP,r8",    2,      true},      // 0xE8
        {JP_HL,         "JP HL",        1,      false},     // 0xE9
        {LD_a16_A,      "LD (a16),A",   3,      true},      // 0xEA
        {NULL,          "",             0,      false},     // 0xEB
        {NULL,          "",             0,      false},     // 0xEC
        {NULL,          "",             0,      false},     // 0xED
        {XOR_A_d8,      "XOR A,d8",     2,      true},      // 0xEE
        {RST_28H,       "RST 28H",      1,      false},     // 0xEF
        {LDH_A_a8,      "LDH A,(a8)",   2,      true},      // 0xF0
        {POP_AF,        "POP AF",       1,      true},      // 0xF1
        {LD_A_pC,       "LD A,(C)",     1,      true},      // 0xF2
        {DI,            "DI",           1,      true},      // 0xF3
        {NULL,          "",             0,      false},     // 0xF4
        {PUSH_AF,       "PUSH AF",      1,      true},      // 0xF5
        {OR_A_d8,       "OR A,d8",      2,      true},      // 0xF6
        {RST_30H,       "RST 30H",      1,      false},     // 0xF7
        {LD_HL_SP_r8,   "LD HL,SP+r8",  2,      true},      // 0xF8
        {LD_SP_HL,      "LD SP,HL",     1,      false},     // 0xF9
        {LD_A_a16,      "LD A,(a16)",   3,      true},      // 0xFA
        {EI,            "EI",           1,      true},      // 0xFB
        {NULL,          "",             0,      false},     // 0xFC
        {NULL,          "",             0,      false},     // 0xFD
        {CP_A_d8,       "CP A,d8",      2,      true},      // 0xFE
        {RST_38H,       "RST 38H",      1,      false},     // 0xFF
};
