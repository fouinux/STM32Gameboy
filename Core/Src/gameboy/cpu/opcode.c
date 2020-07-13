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
#define MACRO_LD_r1_r2(r1, r2) \
static void LD_##r1##_##r2(uint8_t optcode) \
{ \
    core_reg.r1 = core_reg.r2; \
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

// Macro: LD r1, (HL)
#define MACRO_LD_r1_HL(r1) \
static void LD_##r1##_HL(uint8_t optcode) \
{ \
    core_reg.r1 = mem_read_u8(core_reg.HL); \
}

MACRO_LD_r1_HL(B);        // LD B, (HL)
MACRO_LD_r1_HL(C);        // LD C, (HL)
MACRO_LD_r1_HL(D);        // LD D, (HL)
MACRO_LD_r1_HL(E);        // LD E, (HL)
MACRO_LD_r1_HL(H);        // LD H, (HL)
MACRO_LD_r1_HL(L);        // LD L, (HL)
MACRO_LD_r1_HL(A);        // LD A, (HL)

// Macro: LD (HL), r1
#define MACRO_LD_HL_r1(r1) \
static void LD_HL_##r1(uint8_t optcode) \
{ \
    mem_write_u8(core_reg.HL, core_reg.r1); \
}

MACRO_LD_HL_r1(B);        // LD (HL), B
MACRO_LD_HL_r1(C);        // LD (HL), C
MACRO_LD_HL_r1(D);        // LD (HL), D
MACRO_LD_HL_r1(E);        // LD (HL), E
MACRO_LD_HL_r1(H);        // LD (HL), H
MACRO_LD_HL_r1(L);        // LD (HL), L
MACRO_LD_HL_r1(A);        // LD (HL), A

// Macro: LD (HL), r1
#define MACRO_LD_r1_d8(r1) \
static void LD_##r1##_d8(uint8_t optcode) \
{ \
    core_reg.r1 = mem_read_u8(core_reg.PC + 1); \
}

MACRO_LD_r1_d8(B);        // LD B, d8
MACRO_LD_r1_d8(C);        // LD C, d8
MACRO_LD_r1_d8(D);        // LD D, d8
MACRO_LD_r1_d8(E);        // LD E, d8
MACRO_LD_r1_d8(H);        // LD H, d8
MACRO_LD_r1_d8(L);        // LD L, d8
MACRO_LD_r1_d8(A);        // LD A, d8

// LD (HL), d8
static void LD_HL_d8(uint8_t optcode)
{
    mem_write_u8(core_reg.HL, mem_read_u8(core_reg.PC + 1));
}

// Macro: LD (r1), A
#define MACRO_LD_r1_A(r1) \
static void LD_##r1##_A(uint8_t optcode) \
{ \
    mem_write_u8(core_reg.r1, core_reg.A); \
}

MACRO_LD_r1_A(BC);        // LD (BC), A
MACRO_LD_r1_A(DE);        // LD (DE), A

// LD (HL+), A
static void LD_HLp_A(uint8_t optcode)
{
    mem_write_u8(core_reg.HL++, core_reg.A);
}

// LD (HL-), A
static void LD_HLm_A(uint8_t optcode)
{
    mem_write_u8(core_reg.HL--, core_reg.A);
}

// Macro: LD (r1), A
#define MACRO_LD_A_r1(r1) \
static void LD_A_##r1(uint8_t optcode) \
{ \
    core_reg.A = mem_read_u8(core_reg.r1); \
}

MACRO_LD_A_r1(BC);        // LD A, (BC)
MACRO_LD_A_r1(DE);        // LD A, (DE)

// LD A, (HL+)
static void LD_A_HLp(uint8_t optcode)
{
    core_reg.A = mem_read_u8(core_reg.HL++);
}

// LD A, (HL-)
static void LD_A_HLm(uint8_t optcode)
{
    core_reg.A = mem_read_u8(core_reg.HL--);
}


//////////////////////
//   16-bit Load    //
//////////////////////

// Macro: LD r1, d16
#define MACRO_LD_r1_d16(r1) \
static void LD_##r1##_d16(uint8_t optcode) \
{ \
    core_reg.r1 = mem_read_u16(core_reg.PC + 1); \
}

MACRO_LD_r1_d16(BC);        // LD BC, d16
MACRO_LD_r1_d16(DE);        // LD DE, d16
MACRO_LD_r1_d16(HL);        // LD HL, d16
MACRO_LD_r1_d16(SP);        // LD SP, d16

// Load (a16) with SP - LD (a16), SP
void LD_a16_SP(uint8_t opcode)
{
    uint16_t a16 = mem_read_u16(core_reg.PC + 1);

    mem_write_u16(a16, core_reg.SP);
}

//////////////////////
// Arithmetic 8-bit //
//////////////////////

// Macro: ADD A, r1
#define MACRO_ADD_A_r1(r1) \
static void ADD_A_##r1(uint8_t optcode) \
{ \
    uint16_t t = core_reg.A + core_reg.r1; \
    core_reg.F = 0; \
    core_reg.Flags.Z = ((t & 0xFF) == 0);\
    core_reg.Flags.H = ((core_reg.A & 0xF) + (core_reg.r1 & 0xF) > 0xF); \
    core_reg.Flags.C = (t > 0xFF); \
    core_reg.A = t & 0xFF; \
}

MACRO_ADD_A_r1(B);    // ADD A, B
MACRO_ADD_A_r1(C);    // ADD A, C
MACRO_ADD_A_r1(D);    // ADD A, D
MACRO_ADD_A_r1(E);    // ADD A, E
MACRO_ADD_A_r1(H);    // ADD A, H
MACRO_ADD_A_r1(L);    // ADD A, L
MACRO_ADD_A_r1(A);    // ADD A, A

// ADD A, (HL)
static void ADD_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    uint16_t t = core_reg.A + reg;
    core_reg.F = 0;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = ((core_reg.A & 0xF) + (reg & 0xF) > 0xF);
    core_reg.Flags.C = (t > 0xFF);
    core_reg.A = t & 0xFF;
}

// ADD A, d8
static void ADD_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.PC + 1);
    uint16_t t = core_reg.A + d8;
    core_reg.F = 0;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = ((core_reg.A & 0xF) + (d8 & 0xF) > 0xF);
    core_reg.Flags.C = (t > 0xFF);
    core_reg.A = t & 0xFF;
}

// Macro: ADC A, r1
#define MACRO_ADC_A_r1(r1) \
static void ADC_A_##r1(uint8_t optcode) \
{ \
    uint16_t t = core_reg.A + core_reg.r1 + core_reg.Flags.C; \
    core_reg.F = 0; \
    core_reg.Flags.Z = ((t & 0xFF) == 0);\
    core_reg.Flags.H = ((core_reg.A & 0xF) + (core_reg.r1 & 0xF) > 0xF); \
    core_reg.Flags.C = (t > 0xFF); \
    core_reg.A = t & 0xFF; \
}

MACRO_ADC_A_r1(B);    // ADC A, B
MACRO_ADC_A_r1(C);    // ADC A, C
MACRO_ADC_A_r1(D);    // ADC A, D
MACRO_ADC_A_r1(E);    // ADC A, E
MACRO_ADC_A_r1(H);    // ADC A, H
MACRO_ADC_A_r1(L);    // ADC A, L
MACRO_ADC_A_r1(A);    // ADC A, A

// ADC A, (HL)
static void ADC_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    uint16_t t = core_reg.A + reg +  core_reg.Flags.C;
    core_reg.F = 0;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = ((core_reg.A & 0xF) + (reg & 0xF) > 0xF);
    core_reg.Flags.C = (t > 0xFF);
    core_reg.A = t & 0xFF;
}

// ADC A, d8
static void ADC_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.PC + 1);
    uint16_t t = core_reg.A + d8 +  core_reg.Flags.C;
    core_reg.F = 0;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = ((core_reg.A & 0xF) + (d8 & 0xF) > 0xF);
    core_reg.Flags.C = (t > 0xFF);
    core_reg.A = t & 0xFF;
}

// Macro: SUB A, r1
#define MACRO_SUB_A_r1(r1) \
static void SUB_A_##r1(uint8_t optcode) \
{ \
    int16_t t = core_reg.A - core_reg.r1; \
    core_reg.F = 0x40; \
    core_reg.Flags.Z = ((t & 0xFF) == 0);\
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) core_reg.r1 & 0xF) < 0); \
    core_reg.Flags.C = (t < 0); \
    core_reg.A = t & 0xFF; \
}

MACRO_SUB_A_r1(B);    // SUB A, B
MACRO_SUB_A_r1(C);    // SUB A, C
MACRO_SUB_A_r1(D);    // SUB A, D
MACRO_SUB_A_r1(E);    // SUB A, E
MACRO_SUB_A_r1(H);    // SUB A, H
MACRO_SUB_A_r1(L);    // SUB A, L
MACRO_SUB_A_r1(A);    // SUB A, A

// SUB A, (HL)
static void SUB_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    int16_t t = core_reg.A - reg;
    core_reg.F = 0x40;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
    core_reg.Flags.C = (t < 0);
    core_reg.A = t & 0xFF;
}

// SUB A, d8
static void SUB_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.PC + 1);
    int16_t t = core_reg.A - d8;
    core_reg.F = 0x40;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) d8 & 0xF) < 0);
    core_reg.Flags.C = (t < 0);
    core_reg.A = t & 0xFF;
}


// Macro: SBC A, r1
#define MACRO_SBC_A_r1(r1) \
static void SBC_A_##r1(uint8_t optcode) \
{ \
    int16_t t = core_reg.A - core_reg.r1 - core_reg.Flags.C; \
    core_reg.F = 0x40; \
    core_reg.Flags.Z = ((t & 0xFF) == 0);\
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) core_reg.r1 & 0xF) < 0); \
    core_reg.Flags.C = (t < 0); \
    core_reg.A = t & 0xFF; \
}

MACRO_SBC_A_r1(B);    // SBC A, B
MACRO_SBC_A_r1(C);    // SBC A, C
MACRO_SBC_A_r1(D);    // SBC A, D
MACRO_SBC_A_r1(E);    // SBC A, E
MACRO_SBC_A_r1(H);    // SBC A, H
MACRO_SBC_A_r1(L);    // SBC A, L
MACRO_SBC_A_r1(A);    // SBC A, A

// SBC A, (HL)
static void SBC_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    int16_t t = core_reg.A - reg - core_reg.Flags.C;
    core_reg.F = 0x40;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
    core_reg.Flags.C = (t < 0);
    core_reg.A = t & 0xFF;
}

// SBC A, d8
static void SBC_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.PC + 1);
    int16_t t = core_reg.A - d8 - core_reg.Flags.C;
    core_reg.F = 0x40;
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) d8 & 0xF) < 0);
    core_reg.Flags.C = (t < 0);
    core_reg.A = t & 0xFF;
}

// Macro: AND A, r1
#define MACRO_AND_A_r1(r1) \
static void AND_A_##r1(uint8_t optcode) \
{ \
    core_reg.A &= core_reg.r1; \
    core_reg.F = 0x20; /* N = 0, H = 1, C = 0 */ \
    core_reg.Flags.Z = (core_reg.A == 0);\
}

MACRO_AND_A_r1(B);    // AND A, B
MACRO_AND_A_r1(C);    // AND A, C
MACRO_AND_A_r1(D);    // AND A, D
MACRO_AND_A_r1(E);    // AND A, E
MACRO_AND_A_r1(H);    // AND A, H
MACRO_AND_A_r1(L);    // AND A, L
MACRO_AND_A_r1(A);    // AND A, A

// AND A, (HL)
static void AND_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    core_reg.A &= reg;
    core_reg.F = 0x20; /* N = 0, H = 1, C = 0 */
    core_reg.Flags.Z = (core_reg.A == 0);
}

// AND A, d8
static void AND_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.PC + 1);
    core_reg.A &= d8;
    core_reg.F = 0x20; /* N = 0, H = 1, C = 0 */
    core_reg.Flags.Z = (core_reg.A == 0);
}


// Macro: XOR A, r1
#define MACRO_XOR_A_r1(r1) \
static void XOR_A_##r1(uint8_t optcode) \
{ \
    core_reg.A ^= core_reg.r1; \
    core_reg.F = 0x00; /* N = 0, H = 0, C = 0 */ \
    core_reg.Flags.Z = (core_reg.A == 0);\
}

MACRO_XOR_A_r1(B);    // XOR A, B
MACRO_XOR_A_r1(C);    // XOR A, C
MACRO_XOR_A_r1(D);    // XOR A, D
MACRO_XOR_A_r1(E);    // XOR A, E
MACRO_XOR_A_r1(H);    // XOR A, H
MACRO_XOR_A_r1(L);    // XOR A, L
MACRO_XOR_A_r1(A);    // XOR A, A

// XOR A, (HL)
static void XOR_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    core_reg.A ^= reg;
    core_reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    core_reg.Flags.Z = (core_reg.A == 0);
}

// XOR A, d8
static void XOR_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.PC + 1);
    core_reg.A ^= d8;
    core_reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    core_reg.Flags.Z = (core_reg.A == 0);
}

// Macro: OR A, r1
#define MACRO_OR_A_r1(r1) \
static void OR_A_##r1(uint8_t optcode) \
{ \
    core_reg.A |= core_reg.r1; \
    core_reg.F = 0x00; /* N = 0, H = 0, C = 0 */ \
    core_reg.Flags.Z = (core_reg.A == 0);\
}

MACRO_OR_A_r1(B);    // OR A, B
MACRO_OR_A_r1(C);    // OR A, C
MACRO_OR_A_r1(D);    // OR A, D
MACRO_OR_A_r1(E);    // OR A, E
MACRO_OR_A_r1(H);    // OR A, H
MACRO_OR_A_r1(L);    // OR A, L
MACRO_OR_A_r1(A);    // OR A, A

// OR A, (HL)
static void OR_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    core_reg.A |= reg;
    core_reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    core_reg.Flags.Z = (core_reg.A == 0);
}

// OR A, d8
static void OR_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.PC + 1);
    core_reg.A |= d8;
    core_reg.F = 0x00; /* N = 0, H = 0, C = 0 */
    core_reg.Flags.Z = (core_reg.A == 0);
}

// Macro: CP A, r1
#define MACRO_CP_A_r1(r1) \
static void CP_A_##r1(uint8_t optcode) \
{ \
    int16_t t = core_reg.A - core_reg.r1; \
    core_reg.F = 0x40; /* N = 1 */ \
    core_reg.Flags.Z = ((t & 0xFF) == 0);\
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) core_reg.r1 & 0xF) < 0); \
    core_reg.Flags.C = (t < 0); \
}

MACRO_CP_A_r1(B);    // CP A, B
MACRO_CP_A_r1(C);    // CP A, C
MACRO_CP_A_r1(D);    // CP A, D
MACRO_CP_A_r1(E);    // CP A, E
MACRO_CP_A_r1(H);    // CP A, H
MACRO_CP_A_r1(L);    // CP A, L
MACRO_CP_A_r1(A);    // CP A, A

// CP A, (HL)
static void CP_A_HL(uint8_t optcode)
{
    uint8_t reg = mem_read_u8(core_reg.HL);
    int16_t t = core_reg.A - reg;
    core_reg.F = 0x40; /* N = 1 */
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) reg & 0xF) < 0);
    core_reg.Flags.C = (t < 0);
}

// CP A, d8
static void CP_A_d8(uint8_t optcode)
{
    uint8_t d8 = mem_read_u8(core_reg.HL);
    int16_t t = core_reg.A - d8;
    core_reg.F = 0x40; /* N = 1 */
    core_reg.Flags.Z = ((t & 0xFF) == 0);
    core_reg.Flags.H = (((int8_t) core_reg.A & 0xF) - ((int8_t) d8 & 0xF) < 0);
    core_reg.Flags.C = (t < 0);
}

// Macro: INC r1
#define MACRO_INC_r1(r1) \
static void INC_##r1(uint8_t optcode) \
{ \
    core_reg.r1++; \
    core_reg.Flags.Z = (core_reg.r1 == 0x00); \
    core_reg.Flags.N = 0; \
    core_reg.Flags.H = ((core_reg.r1 & 0x0F) == 0x00); \
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
static void INC_pHL(uint8_t optcode)
{
    uint8_t t = mem_read_u8(core_reg.HL);

    t++;
    core_reg.Flags.Z = (t == 0x00);
    core_reg.Flags.N = 0;
    core_reg.Flags.H = ((t & 0x0F) == 0x00);

    mem_write_u8(core_reg.HL, t);
}

// Macro: DEC r1
#define MACRO_DEC_r1(r1) \
static void DEC_##r1(uint8_t optcode) \
{ \
    core_reg.r1--; \
    core_reg.Flags.Z = (core_reg.r1 == 0x00); \
    core_reg.Flags.N = 1; \
    core_reg.Flags.H = ((core_reg.r1 & 0x0F) == 0x0F); \
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
static void DEC_pHL(uint8_t optcode)
{
    uint8_t t = mem_read_u8(core_reg.HL);

    t--;
    core_reg.Flags.Z = (t == 0x00);
    core_reg.Flags.N = 1;
    core_reg.Flags.H = ((t & 0x0F) == 0x0F);

    mem_write_u8(core_reg.HL, t);
}

//////////////////////
// Arithmetic 16bit //
//////////////////////

// Macro: INC r1
#define MACRO_INC_r1(r1) \
static void INC_##r1(uint8_t optcode) \
{ \
    core_reg.r1++; \
}

MACRO_INC_r1(BC);    // INC BC
MACRO_INC_r1(DE);    // INC DE
MACRO_INC_r1(HL);    // INC HL
MACRO_INC_r1(SP);    // INC SP

// Macro: DEC r1
#define MACRO_DEC_r1(r1) \
static void DEC_##r1(uint8_t optcode) \
{ \
    core_reg.r1--; \
}

MACRO_DEC_r1(BC);    // DEC BC
MACRO_DEC_r1(DE);    // DEC DE
MACRO_DEC_r1(HL);    // DEC HL
MACRO_DEC_r1(SP);    // DEC SP

// Macro: ADD HL r1
#define MACRO_ADD_HL_r1(r1) \
static void ADD_HL_##r1(uint8_t optcode) \
{ \
    uint32_t result = (uint32_t) core_reg.HL + (uint32_t) core_reg.r1; \
 \
    core_reg.Flags.N = 0; \
    core_reg.Flags.C = (result > 0xFFFF); \
 \
    if ((core_reg.HL & 0xFFF) + (core_reg.r1 & 0xFFF) > 0xFFF) \
        core_reg.Flags.H = 1; \
 \
    core_reg.HL = result & 0xFFFF; \
}

MACRO_ADD_HL_r1(BC);    // ADD HL, BC
MACRO_ADD_HL_r1(DE);    // ADD HL, DE
MACRO_ADD_HL_r1(HL);    // ADD HL, HL
MACRO_ADD_HL_r1(SP);    // ADD HL, SP

#undef MACRO_ADD_HL_r1


//////////////////////
//       Misc       //
//////////////////////

// Complement A register - CPL
static void CPL(uint8_t opcode)
{
    core_reg.F |= 0x90; // Set N & H
    core_reg.A ^= 0xFF;
}

// Complement Carry Flag - CCF
static void CCF(uint8_t opcode)
{
    core_reg.F &= 0x90; // Reset N & H
    core_reg.Flags.C = !core_reg.Flags.C;
}

// Set Carry Flag - SCF
static void SCF(uint8_t opcode)
{
    core_reg.F &= 0x90; // Reset N & H
    core_reg.Flags.C = 1;
}


// NOP
static void NOP(uint8_t opcode)
{
    // Do nothing for one cycle
}

//////////////////////
// Rotates & Shifts //
//////////////////////

// Rotate A Left
static void RLCA(uint8_t opcode)
{
    ll_RLC(&core_reg.A);

    // Update flags
    core_reg.Flags.Z = 0;
}

// Rotate A Left through Carry flag
static void RLA(uint8_t opcode)
{
    ll_RL(&core_reg.A);

    // Update flags
    core_reg.Flags.Z = 0;
}

// Rotate A Right
static void RRCA(uint8_t opcode)
{
    ll_RRC(&core_reg.A);

    // Update flags
    core_reg.Flags.Z = 0;
}

// Rotate A Right through Carry flag
static void RRA(uint8_t opcode)
{
    ll_RR(&core_reg.A);

    // Update flags
    core_reg.Flags.Z = 0;
}

//////////////////////
//     Jumps        //
//////////////////////

// Jump relative - JR r8
static void JR_n(uint8_t opcode)
{
    int8_t r8 = mem_read_s8(core_reg.PC + 1);
    core_reg.PC += r8;
}

// JR NZ, r8
static void JR_NZ_r8(uint8_t opcode)
{
    // TODO Add 4 cycles in case of jump?
    int8_t r8 = mem_read_s8(core_reg.PC + 1);

    if(core_reg.Flags.Z == 0)
        core_reg.PC += r8; // Relative jump
    else
        core_reg.PC += 2; // Next opcode
}

// JR Z, r8
static void JR_Z_r8(uint8_t opcode)
{
    // TODO Add 4 cycles in case of jump?
    int8_t r8 = mem_read_s8(core_reg.PC + 1);

    if(core_reg.Flags.Z == 1)
        core_reg.PC += r8; // Relative jump
    else
        core_reg.PC += 2; // Next opcode
}

// JR NC, r8
static void JR_NC_r8(uint8_t opcode)
{
    // TODO Add 4 cycles in case of jump?
    int8_t r8 = mem_read_s8(core_reg.PC + 1);

    if(core_reg.Flags.C == 0)
        core_reg.PC += r8; // Relative jump
    else
        core_reg.PC += 2; // Next opcode
}

// JR C, r8
static void JR_C_r8(uint8_t opcode)
{
    // TODO Add 4 cycles in case of jump?
    int8_t r8 = mem_read_s8(core_reg.PC + 1);

    if(core_reg.Flags.C == 1)
        core_reg.PC += r8; // Relative jump
    else
        core_reg.PC += 2; // Next opcode
}

struct opcode_t opcodeList[256] =
{
    {NOP,			1,    4,    true},		// 0x00
    {LD_BC_d16,     3,    12,   true},    	// 0x01
    {LD_BC_A,       1,    8,    true},      // 0x02
    {INC_BC,        1,    8,    true},      // 0x03
    {INC_B,         1,    4,    true},      // 0x04
    {DEC_B,         1,    4,    true},      // 0x05
    {LD_B_d8,       2,    8,    true},      // 0x06
    {RLCA,          1,    4,    true},      // 0x07
    {LD_a16_SP,     3,    20,   true},      // 0x08
    {ADD_HL_BC,     1,    8,    true},      // 0x09
    {LD_A_BC,       1,    8,    true},      // 0x0A
    {DEC_BC,        1,    8,    true},      // 0x0B
    {INC_C,         1,    4,    true},      // 0x0C
    {DEC_C,         1,    4,    true},      // 0x0D
    {LD_C_d8,       2,    8,    true},      // 0x0E
    {RRCA,			1,    4,    true},      // 0x0F
    {LD_DE_d16,     3,    12,   true},    	// 0x11
    {LD_DE_A,       1,    8,    true},      // 0x12
    {INC_DE,        1,    8,    true},      // 0x13
    {INC_D,         1,    4,    true},      // 0x14
    {DEC_D,         1,    4,    true},      // 0x15
    {LD_D_d8,       2,    8,    true},      // 0x16
    {RLA,           1,    4,    true},      // 0x17
    {JR_n,          2,    12,   false},     // 0x18
    {ADD_HL_DE,     1,    8,    true},      // 0x19
    {LD_A_DE,       1,    8,    true},      // 0x1A
    {DEC_DE,        1,    8,    true},      // 0x1B
    {INC_E,         1,    4,    true},      // 0x1C
    {DEC_E,         1,    4,    true},      // 0x1D
    {LD_E_d8,       2,    8,    true},      // 0x1E
    {RRA,           1,    4,    true},      // 0x1F
    {JR_NZ_r8,      2,    8,    false},     // 0x20
    {LD_HL_d16,     3,    12,   true},      // 0x21
    {LD_HLp_A,      1,    8,    true},      // 0x22
    {INC_HL,        1,    8,    true},      // 0x23
    {INC_H,         1,    4,    true},      // 0x24
    {DEC_H,         1,    4,    true},      // 0x25
    {LD_H_d8,       2,    8,    true},      // 0x26

    {NULL,          1,    4,    true},      // 0x27 TODO DAA

    {JR_Z_r8,       2,    8,    false},     // 0x28
    {ADD_HL_HL,     1,    8,    true},      // 0x29
    {LD_A_HLp,      1,    8,    true},      // 0x2A
    {DEC_HL,        1,    8,    true},      // 0x2B
    {INC_L,         1,    4,    true},      // 0x2C
    {DEC_L,         1,    4,    true},      // 0x2D
    {LD_L_d8,       2,    8,    true},      // 0x2E
    {CPL,           1,    4,    true},      // 0x2F
    {JR_NC_r8,      2,    8,    false},     // 0x30
    {LD_SP_d16,     3,    12,   true},      // 0x31
    {LD_HLm_A,      1,    8,    true},      // 0x32
    {INC_SP,        1,    8,    true},      // 0x33
    {INC_pHL,       1,    12,   true},      // 0x34
    {DEC_pHL,       1,    12,   true},      // 0x35
    {LD_HL_d8,      1,    8,    true},      // 0x36
    {SCF,           1,    4,    true},      // 0x37
    {JR_C_r8,       2,    8,    false},     // 0x38
    {ADD_HL_SP,     1,    8,    true},      // 0x39
    {LD_A_HLm,      1,    8,    true},      // 0x3A
    {DEC_SP,        1,    8,    true},      // 0x3B
    {INC_A,         1,    4,    true},      // 0x3C
    {DEC_A,         1,    4,    true},      // 0x3D
    {LD_A_d8,       2,    8,    true},      // 0x3E
    {CCF,           1,    4,    true},      // 0x3F
    {LD_B_B,        1,    4,    true},      // 0x40
    {LD_B_C,        1,    4,    true},      // 0x41
    {LD_B_D,        1,    4,    true},      // 0x42
    {LD_B_E,        1,    4,    true},      // 0x43
    {LD_B_H,        1,    4,    true},      // 0x44
    {LD_B_L,        1,    4,    true},      // 0x45
    {LD_B_HL,       1,    8,    true},      // 0x46
    {LD_B_A,        1,    4,    true},      // 0x47
    {LD_C_B,        1,    4,    true},      // 0x48
    {LD_C_C,        1,    4,    true},      // 0x49
    {LD_C_D,        1,    4,    true},      // 0x4A
    {LD_C_E,        1,    4,    true},      // 0x4B
    {LD_C_H,        1,    4,    true},      // 0x4C
    {LD_C_L,        1,    4,    true},      // 0x4D
    {LD_C_HL,       1,    8,    true},      // 0x4E
    {LD_C_A,        1,    4,    true},      // 0x4F
    {LD_D_B,        1,    4,    true},      // 0x50
    {LD_D_C,        1,    4,    true},      // 0x51
    {LD_D_D,        1,    4,    true},      // 0x52
    {LD_D_E,        1,    4,    true},      // 0x53
    {LD_D_H,        1,    4,    true},      // 0x54
    {LD_D_L,        1,    4,    true},      // 0x55
    {LD_D_HL,       1,    8,    true},      // 0x56
    {LD_D_A,        1,    4,    true},      // 0x57
    {LD_E_B,        1,    4,    true},      // 0x58
    {LD_E_C,        1,    4,    true},      // 0x59
    {LD_E_D,        1,    4,    true},      // 0x5A
    {LD_E_E,        1,    4,    true},      // 0x5B
    {LD_E_H,        1,    4,    true},      // 0x5C
    {LD_E_L,        1,    4,    true},      // 0x5D
    {LD_E_HL,       1,    8,    true},      // 0x5E
    {LD_E_A,        1,    4,    true},      // 0x5F
    {LD_H_B,        1,    4,    true},      // 0x60
    {LD_H_C,        1,    4,    true},      // 0x61
    {LD_H_D,        1,    4,    true},      // 0x62
    {LD_H_E,        1,    4,    true},      // 0x63
    {LD_H_H,        1,    4,    true},      // 0x64
    {LD_H_L,        1,    4,    true},      // 0x65
    {LD_H_HL,       1,    8,    true},      // 0x66
    {LD_H_A,        1,    4,    true},      // 0x67
    {LD_L_B,        1,    4,    true},      // 0x68
    {LD_L_C,        1,    4,    true},      // 0x69
    {LD_L_D,        1,    4,    true},      // 0x6A
    {LD_L_E,        1,    4,    true},      // 0x6B
    {LD_L_H,        1,    4,    true},      // 0x6C
    {LD_L_L,        1,    4,    true},      // 0x6D
    {LD_L_HL,       1,    8,    true},      // 0x6E
    {LD_L_A,        1,    4,    true},      // 0x6F
    {LD_HL_B,       1,    8,    true},      // 0x70
    {LD_HL_C,       1,    8,    true},      // 0x71
    {LD_HL_D,       1,    8,    true},      // 0x72
    {LD_HL_E,       1,    8,    true},      // 0x73
    {LD_HL_H,       1,    8,    true},      // 0x74
    {LD_HL_L,       1,    8,    true},      // 0x75

    {NULL,          1,    4,    true},      // 0x76 // TODO HALT

    {LD_HL_A,       1,    8,    true},      // 0x77
    {LD_A_B,        1,    4,    true},      // 0x78
    {LD_A_C,        1,    4,    true},      // 0x79
    {LD_A_D,        1,    4,    true},      // 0x7A
    {LD_A_E,        1,    4,    true},      // 0x7B
    {LD_A_H,        1,    4,    true},      // 0x7C
    {LD_A_L,        1,    4,    true},      // 0x7D
    {LD_A_HL,       1,    8,    true},      // 0x7E
    {LD_A_A,        1,    4,    true},      // 0x7F
    {ADD_A_B,       1,    4,    true},      // 0x80
    {ADD_A_C,       1,    4,    true},      // 0x81
    {ADD_A_D,       1,    4,    true},      // 0x82
    {ADD_A_E,       1,    4,    true},      // 0x83
    {ADD_A_H,       1,    4,    true},      // 0x84
    {ADD_A_L,       1,    4,    true},      // 0x85
    {ADD_A_HL,      1,    8,    true},      // 0x86
    {ADD_A_A,       1,    4,    true},      // 0x87
    {ADC_A_B,       1,    4,    true},      // 0x88
    {ADC_A_C,       1,    4,    true},      // 0x89
    {ADC_A_D,       1,    4,    true},      // 0x8A
    {ADC_A_E,       1,    4,    true},      // 0x8B
    {ADC_A_H,       1,    4,    true},      // 0x8C
    {ADC_A_L,       1,    4,    true},      // 0x8D
    {ADC_A_HL,      1,    8,    true},      // 0x8E
    {ADC_A_A,       1,    4,    true},      // 0x8F
    {SUB_A_B,       1,    4,    true},      // 0x90
    {SUB_A_C,       1,    4,    true},      // 0x91
    {SUB_A_D,       1,    4,    true},      // 0x92
    {SUB_A_E,       1,    4,    true},      // 0x93
    {SUB_A_H,       1,    4,    true},      // 0x94
    {SUB_A_L,       1,    4,    true},      // 0x95
    {SUB_A_HL,      1,    8,    true},      // 0x96
    {SUB_A_A,       1,    4,    true},      // 0x97
    {SBC_A_B,       1,    4,    true},      // 0x98
    {SBC_A_C,       1,    4,    true},      // 0x99
    {SBC_A_D,       1,    4,    true},      // 0x9A
    {SBC_A_E,       1,    4,    true},      // 0x9B
    {SBC_A_H,       1,    4,    true},      // 0x9C
    {SBC_A_L,       1,    4,    true},      // 0x9D
    {SBC_A_HL,      1,    8,    true},      // 0x9E
    {SBC_A_A,       1,    4,    true},      // 0x9F
    {AND_A_B,       1,    4,    true},      // 0xA0
    {AND_A_C,       1,    4,    true},      // 0xA1
    {AND_A_D,       1,    4,    true},      // 0xA2
    {AND_A_E,       1,    4,    true},      // 0xA3
    {AND_A_H,       1,    4,    true},      // 0xA4
    {AND_A_L,       1,    4,    true},      // 0xA5
    {AND_A_HL,      1,    8,    true},      // 0xA6
    {AND_A_A,       1,    4,    true},      // 0xA7
    {XOR_A_B,       1,    4,    true},      // 0xA8
    {XOR_A_C,       1,    4,    true},      // 0xA9
    {XOR_A_D,       1,    4,    true},      // 0xAA
    {XOR_A_E,       1,    4,    true},      // 0xAB
    {XOR_A_H,       1,    4,    true},      // 0xAC
    {XOR_A_L,       1,    4,    true},      // 0xAD
    {XOR_A_HL,      1,    8,    true},      // 0xAE
    {XOR_A_A,       1,    4,    true},      // 0xAF
    {OR_A_B,        1,    4,    true},      // 0xB0
    {OR_A_C,        1,    4,    true},      // 0xB1
    {OR_A_D,        1,    4,    true},      // 0xB2
    {OR_A_E,        1,    4,    true},      // 0xB3
    {OR_A_H,        1,    4,    true},      // 0xB4
    {OR_A_L,        1,    4,    true},      // 0xB5
    {OR_A_HL,       1,    8,    true},      // 0xB6
    {OR_A_A,        1,    4,    true},      // 0xB7
    {CP_A_B,        1,    4,    true},      // 0xB8
    {CP_A_C,        1,    4,    true},      // 0xB9
    {CP_A_D,        1,    4,    true},      // 0xBA
    {CP_A_E,        1,    4,    true},      // 0xBB
    {CP_A_H,        1,    4,    true},      // 0xBC
    {CP_A_L,        1,    4,    true},      // 0xBD
    {CP_A_HL,       1,    8,    true},      // 0xBE
    {CP_A_A,        1,    4,    true},      // 0xBF
};
