/*
 * opcode_cb.c
 *
 *  Created on: 7 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include "cpu.h"
#include "mem.h"
#include "opcode_cb.h"
#include "opcode.h"
#include <stdio.h>

// Macro: RLC r1
#define MACRO_RLC_r1(r1) \
static uint8_t RLC_##r1(void) \
{ \
    cpu.reg.r1 = (cpu.reg.r1 << 1) | (cpu.reg.r1 >> 7); \
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0); \
    cpu.reg.Flags.C = cpu.reg.r1 & 0x01; \
    return 2; \
}

MACRO_RLC_r1(B);    // RLC B
MACRO_RLC_r1(C);    // RLC C
MACRO_RLC_r1(D);    // RLC D
MACRO_RLC_r1(E);    // RLC E
MACRO_RLC_r1(H);    // RLC H
MACRO_RLC_r1(L);    // RLC L
MACRO_RLC_r1(A);    // RLC A

#undef MACRO_RLC_r1

// RLC (HL)
static uint8_t RLC_HL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    t = (t << 1) | (t >> 7);
    cpu.reg.F = 0x00;
    cpu.reg.Flags.Z = (t == 0);
    cpu.reg.Flags.C = t & 0x01;

    mem_write_u8(cpu.reg.HL, t);
    return 4;
}

// Macro: RL r1
#define MACRO_RL_r1(r1) \
static uint8_t RL_##r1(void) \
{ \
    uint16_t t = (cpu.reg.r1 << 1) | cpu.reg.Flags.C; \
    cpu.reg.r1 = t & 0xFF; \
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0); \
    cpu.reg.Flags.C = (t > 0xFF); \
    return 2; \
}

MACRO_RL_r1(B);    // RL B
MACRO_RL_r1(C);    // RL C
MACRO_RL_r1(D);    // RL D
MACRO_RL_r1(E);    // RL E
MACRO_RL_r1(H);    // RL H
MACRO_RL_r1(L);    // RL L
MACRO_RL_r1(A);    // RL A

#undef MACRO_RL_r1

// RL (HL)
static uint8_t RL_HL(void)
{
    uint16_t t = (uint16_t) mem_read_u8(cpu.reg.HL);

    t = (t << 1) | cpu.reg.Flags.C;
    cpu.reg.F = 0x00;
    cpu.reg.Flags.Z = ((t & 0xFF) == 0);
    cpu.reg.Flags.C = (t > 0xFF);

    mem_write_u8(cpu.reg.HL, t & 0xFF);
    return 4;
}

// Macro: RRC r1
#define MACRO_RRC_r1(r1) \
static uint8_t RRC_##r1(void) \
{ \
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.C = cpu.reg.r1 & 0x01; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0); \
    cpu.reg.r1 = (cpu.reg.r1 >> 1) | (cpu.reg.r1 << 7); \
    return 2; \
}

MACRO_RRC_r1(B);    // RRC B
MACRO_RRC_r1(C);    // RRC C
MACRO_RRC_r1(D);    // RRC D
MACRO_RRC_r1(E);    // RRC E
MACRO_RRC_r1(H);    // RRC H
MACRO_RRC_r1(L);    // RRC L
MACRO_RRC_r1(A);    // RRC A

#undef MACRO_RRC_r1

// RRC (HL)
static uint8_t RRC_HL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = t & 0x01;
    cpu.reg.Flags.Z = (t == 0);
    t = (t >> 1) | (t << 7);

    mem_write_u8(cpu.reg.HL, t);
    return 4;
}

// Macro: RR r1
#define MACRO_RR_r1(r1) \
static uint8_t RR_##r1(void) \
{ \
    uint8_t t = (cpu.reg.r1 >> 1) | (cpu.reg.Flags.C << 7); \
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.C = cpu.reg.r1 & 0x01; \
    cpu.reg.Flags.Z = (t == 0); \
    cpu.reg.r1 = t; \
    return 2; \
}

MACRO_RR_r1(B);    // RR B
MACRO_RR_r1(C);    // RR C
MACRO_RR_r1(D);    // RR D
MACRO_RR_r1(E);    // RR E
MACRO_RR_r1(H);    // RR H
MACRO_RR_r1(L);    // RR L
MACRO_RR_r1(A);    // RR A

#undef MACRO_RR_r1

// RR (HL)
static uint8_t RR_HL(void)
{
    uint8_t u8 = mem_read_u8(cpu.reg.HL);

    uint8_t t = (u8 >> 1) | (cpu.reg.Flags.C << 7);
    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = u8 & 0x01;
    cpu.reg.Flags.Z = (t == 0);

    mem_write_u8(cpu.reg.HL, t);
    return 4;
}

// Macro: SLA r1
#define MACRO_SLA_r1(r1) \
static uint8_t SLA_##r1(void) \
{ \
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.C = cpu.reg.r1 >> 7; \
    cpu.reg.r1 <<= 1; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0); \
    return 2; \
}

MACRO_SLA_r1(B);    // SLA B
MACRO_SLA_r1(C);    // SLA C
MACRO_SLA_r1(D);    // SLA D
MACRO_SLA_r1(E);    // SLA E
MACRO_SLA_r1(H);    // SLA H
MACRO_SLA_r1(L);    // SLA L
MACRO_SLA_r1(A);    // SLA A

#undef MACRO_SLA_r1

// SLA (HL)
static uint8_t SLA_HL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = t >> 7;
    t <<= 1;
    cpu.reg.Flags.Z = (t == 0);

    mem_write_u8(cpu.reg.HL, t);
    return 4;
}

// Macro: SRA r1
#define MACRO_SRA_r1(r1) \
static uint8_t SRA_##r1(void) \
{ \
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.C = cpu.reg.r1 & 0x01; \
    cpu.reg.r1 = (cpu.reg.r1 & 0x80) | (cpu.reg.r1 >>  1); \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0); \
    return 2; \
}

MACRO_SRA_r1(B);    // SRA B
MACRO_SRA_r1(C);    // SRA C
MACRO_SRA_r1(D);    // SRA D
MACRO_SRA_r1(E);    // SRA E
MACRO_SRA_r1(H);    // SRA H
MACRO_SRA_r1(L);    // SRA L
MACRO_SRA_r1(A);    // SRA A

#undef MACRO_SRA_r1

// SRA (HL)
static uint8_t SRA_HL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = t & 0x01;
    t = (t & 0x80) | (t >> 1);
    cpu.reg.Flags.Z = (t == 0);

    mem_write_u8(cpu.reg.HL, t);
    return 4;
}

// Macro: SWAP r1
#define MACRO_SWAP_r1(r1) \
static uint8_t SWAP_##r1(void) \
{ \
    cpu.reg.r1 = (cpu.reg.r1 >> 4) | (cpu.reg.r1 << 4);\
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0); \
    return 2; \
}

MACRO_SWAP_r1(B);    // SWAP B
MACRO_SWAP_r1(C);    // SWAP C
MACRO_SWAP_r1(D);    // SWAP D
MACRO_SWAP_r1(E);    // SWAP E
MACRO_SWAP_r1(H);    // SWAP H
MACRO_SWAP_r1(L);    // SWAP L
MACRO_SWAP_r1(A);    // SWAP A

#undef MACRO_SWAP_r1

// SWAP (HL)
static uint8_t SWAP_HL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    t = (t >> 4) | (t << 4);
    cpu.reg.F = 0x00;
    cpu.reg.Flags.Z = (t == 0);

    mem_write_u8(cpu.reg.HL, t);
    return 4;
}

// Macro: SRL r1
#define MACRO_SRL_r1(r1) \
static uint8_t SRL_##r1(void) \
{ \
    cpu.reg.F = 0x00; \
    cpu.reg.Flags.C = (cpu.reg.r1 & 0x01); \
    cpu.reg.r1 >>= 1; \
    cpu.reg.Flags.Z = (cpu.reg.r1 == 0); \
    return 2; \
}

MACRO_SRL_r1(B);    // SRL B
MACRO_SRL_r1(C);    // SRL C
MACRO_SRL_r1(D);    // SRL D
MACRO_SRL_r1(E);    // SRL E
MACRO_SRL_r1(H);    // SRL H
MACRO_SRL_r1(L);    // SRL L
MACRO_SRL_r1(A);    // SRL A

#undef MACRO_SRL_r1

// SRL (HL)
static uint8_t SRL_HL(void)
{
    uint8_t t = mem_read_u8(cpu.reg.HL);

    cpu.reg.F = 0x00;
    cpu.reg.Flags.C = (t & 0x01);
    t >>= 1;
    cpu.reg.Flags.Z = (t == 0);

    mem_write_u8(cpu.reg.HL, t);
    return 4;
}

// Macro: BIT n, r1
#define MACRO_BIT_n_r1(n, r1) \
static uint8_t BIT_##n##_##r1(void) \
{ \
    cpu.reg.Flags.Z = ((cpu.reg.r1 & (1 << n)) == 0); \
    cpu.reg.Flags.N = 0; \
    cpu.reg.Flags.H = 1; \
    return 2; \
}

MACRO_BIT_n_r1(0, B);    // BIT 0, B
MACRO_BIT_n_r1(0, C);    // BIT 0, C
MACRO_BIT_n_r1(0, D);    // BIT 0, D
MACRO_BIT_n_r1(0, E);    // BIT 0, E
MACRO_BIT_n_r1(0, H);    // BIT 0, H
MACRO_BIT_n_r1(0, L);    // BIT 0, L
MACRO_BIT_n_r1(0, A);    // BIT 0, A

MACRO_BIT_n_r1(1, B);    // BIT 1, B
MACRO_BIT_n_r1(1, C);    // BIT 1, C
MACRO_BIT_n_r1(1, D);    // BIT 1, D
MACRO_BIT_n_r1(1, E);    // BIT 1, E
MACRO_BIT_n_r1(1, H);    // BIT 1, H
MACRO_BIT_n_r1(1, L);    // BIT 1, L
MACRO_BIT_n_r1(1, A);    // BIT 1, A

MACRO_BIT_n_r1(2, B);    // BIT 2, B
MACRO_BIT_n_r1(2, C);    // BIT 2, C
MACRO_BIT_n_r1(2, D);    // BIT 2, D
MACRO_BIT_n_r1(2, E);    // BIT 2, E
MACRO_BIT_n_r1(2, H);    // BIT 2, H
MACRO_BIT_n_r1(2, L);    // BIT 2, L
MACRO_BIT_n_r1(2, A);    // BIT 2, A

MACRO_BIT_n_r1(3, B);    // BIT 3, B
MACRO_BIT_n_r1(3, C);    // BIT 3, C
MACRO_BIT_n_r1(3, D);    // BIT 3, D
MACRO_BIT_n_r1(3, E);    // BIT 3, E
MACRO_BIT_n_r1(3, H);    // BIT 3, H
MACRO_BIT_n_r1(3, L);    // BIT 3, L
MACRO_BIT_n_r1(3, A);    // BIT 3, A

MACRO_BIT_n_r1(4, B);    // BIT 4, B
MACRO_BIT_n_r1(4, C);    // BIT 4, C
MACRO_BIT_n_r1(4, D);    // BIT 4, D
MACRO_BIT_n_r1(4, E);    // BIT 4, E
MACRO_BIT_n_r1(4, H);    // BIT 4, H
MACRO_BIT_n_r1(4, L);    // BIT 4, L
MACRO_BIT_n_r1(4, A);    // BIT 4, A

MACRO_BIT_n_r1(5, B);    // BIT 5, B
MACRO_BIT_n_r1(5, C);    // BIT 5, C
MACRO_BIT_n_r1(5, D);    // BIT 5, D
MACRO_BIT_n_r1(5, E);    // BIT 5, E
MACRO_BIT_n_r1(5, H);    // BIT 5, H
MACRO_BIT_n_r1(5, L);    // BIT 5, L
MACRO_BIT_n_r1(5, A);    // BIT 5, A

MACRO_BIT_n_r1(6, B);    // BIT 6, B
MACRO_BIT_n_r1(6, C);    // BIT 6, C
MACRO_BIT_n_r1(6, D);    // BIT 6, D
MACRO_BIT_n_r1(6, E);    // BIT 6, E
MACRO_BIT_n_r1(6, H);    // BIT 6, H
MACRO_BIT_n_r1(6, L);    // BIT 6, L
MACRO_BIT_n_r1(6, A);    // BIT 6, A

MACRO_BIT_n_r1(7, B);    // BIT 7, B
MACRO_BIT_n_r1(7, C);    // BIT 7, C
MACRO_BIT_n_r1(7, D);    // BIT 7, D
MACRO_BIT_n_r1(7, E);    // BIT 7, E
MACRO_BIT_n_r1(7, H);    // BIT 7, H
MACRO_BIT_n_r1(7, L);    // BIT 7, L
MACRO_BIT_n_r1(7, A);    // BIT 7, A

#undef MACRO_BIT_n_r1

// Macro: BIT n, (HL)
#define MACRO_BIT_n_HL(n) \
static uint8_t BIT_##n##_HL(void) \
{ \
    uint8_t t = mem_read_u8(cpu.reg.HL);\
    cpu.reg.Flags.Z = ((t & (1 << n)) == 0); \
    cpu.reg.Flags.N = 0; \
    cpu.reg.Flags.H = 1; \
    return 4;\
}

MACRO_BIT_n_HL(0);      // BIT 0, (HL)
MACRO_BIT_n_HL(1);      // BIT 1, (HL)
MACRO_BIT_n_HL(2);      // BIT 2, (HL)
MACRO_BIT_n_HL(3);      // BIT 3, (HL)
MACRO_BIT_n_HL(4);      // BIT 4, (HL)
MACRO_BIT_n_HL(5);      // BIT 5, (HL)
MACRO_BIT_n_HL(6);      // BIT 6, (HL)
MACRO_BIT_n_HL(7);      // BIT 7, (HL)

#undef MACRO_BIT_n_HL

// Macro: RES n, r1
#define MACRO_RES_n_r1(n, r1) \
static uint8_t RES_##n##_##r1(void) \
{ \
    cpu.reg.r1 &= ~(1 << n);\
    return 2; \
}

MACRO_RES_n_r1(0, B);    // RES 0, B
MACRO_RES_n_r1(0, C);    // RES 0, C
MACRO_RES_n_r1(0, D);    // RES 0, D
MACRO_RES_n_r1(0, E);    // RES 0, E
MACRO_RES_n_r1(0, H);    // RES 0, H
MACRO_RES_n_r1(0, L);    // RES 0, L
MACRO_RES_n_r1(0, A);    // RES 0, A

MACRO_RES_n_r1(1, B);    // RES 1, B
MACRO_RES_n_r1(1, C);    // RES 1, C
MACRO_RES_n_r1(1, D);    // RES 1, D
MACRO_RES_n_r1(1, E);    // RES 1, E
MACRO_RES_n_r1(1, H);    // RES 1, H
MACRO_RES_n_r1(1, L);    // RES 1, L
MACRO_RES_n_r1(1, A);    // RES 1, A

MACRO_RES_n_r1(2, B);    // RES 2, B
MACRO_RES_n_r1(2, C);    // RES 2, C
MACRO_RES_n_r1(2, D);    // RES 2, D
MACRO_RES_n_r1(2, E);    // RES 2, E
MACRO_RES_n_r1(2, H);    // RES 2, H
MACRO_RES_n_r1(2, L);    // RES 2, L
MACRO_RES_n_r1(2, A);    // RES 2, A

MACRO_RES_n_r1(3, B);    // RES 3, B
MACRO_RES_n_r1(3, C);    // RES 3, C
MACRO_RES_n_r1(3, D);    // RES 3, D
MACRO_RES_n_r1(3, E);    // RES 3, E
MACRO_RES_n_r1(3, H);    // RES 3, H
MACRO_RES_n_r1(3, L);    // RES 3, L
MACRO_RES_n_r1(3, A);    // RES 3, A

MACRO_RES_n_r1(4, B);    // RES 4, B
MACRO_RES_n_r1(4, C);    // RES 4, C
MACRO_RES_n_r1(4, D);    // RES 4, D
MACRO_RES_n_r1(4, E);    // RES 4, E
MACRO_RES_n_r1(4, H);    // RES 4, H
MACRO_RES_n_r1(4, L);    // RES 4, L
MACRO_RES_n_r1(4, A);    // RES 4, A

MACRO_RES_n_r1(5, B);    // RES 5, B
MACRO_RES_n_r1(5, C);    // RES 5, C
MACRO_RES_n_r1(5, D);    // RES 5, D
MACRO_RES_n_r1(5, E);    // RES 5, E
MACRO_RES_n_r1(5, H);    // RES 5, H
MACRO_RES_n_r1(5, L);    // RES 5, L
MACRO_RES_n_r1(5, A);    // RES 5, A

MACRO_RES_n_r1(6, B);    // RES 6, B
MACRO_RES_n_r1(6, C);    // RES 6, C
MACRO_RES_n_r1(6, D);    // RES 6, D
MACRO_RES_n_r1(6, E);    // RES 6, E
MACRO_RES_n_r1(6, H);    // RES 6, H
MACRO_RES_n_r1(6, L);    // RES 6, L
MACRO_RES_n_r1(6, A);    // RES 6, A

MACRO_RES_n_r1(7, B);    // RES 7, B
MACRO_RES_n_r1(7, C);    // RES 7, C
MACRO_RES_n_r1(7, D);    // RES 7, D
MACRO_RES_n_r1(7, E);    // RES 7, E
MACRO_RES_n_r1(7, H);    // RES 7, H
MACRO_RES_n_r1(7, L);    // RES 7, L
MACRO_RES_n_r1(7, A);    // RES 7, A

#undef MACRO_RES_n_r1

// Macro: RES n, (HL)
#define MACRO_RES_n_HL(n) \
static uint8_t RES_##n##_HL(void) \
{ \
    uint8_t t = mem_read_u8(cpu.reg.HL);\
    t &= ~(1 << n);\
    mem_write_u8(cpu.reg.HL, t);\
    return 4; \
}

MACRO_RES_n_HL(0);      // RES 0, (HL)
MACRO_RES_n_HL(1);      // RES 1, (HL)
MACRO_RES_n_HL(2);      // RES 2, (HL)
MACRO_RES_n_HL(3);      // RES 3, (HL)
MACRO_RES_n_HL(4);      // RES 4, (HL)
MACRO_RES_n_HL(5);      // RES 5, (HL)
MACRO_RES_n_HL(6);      // RES 6, (HL)
MACRO_RES_n_HL(7);      // RES 7, (HL)

#undef MACRO_RES_n_HL

// Macro: SET n, r1
#define MACRO_SET_n_r1(n, r1) \
static uint8_t SET_##n##_##r1(void) \
{ \
    cpu.reg.r1 |= (1 << n);\
    return 2; \
}

MACRO_SET_n_r1(0, B);    // SET 0, B
MACRO_SET_n_r1(0, C);    // SET 0, C
MACRO_SET_n_r1(0, D);    // SET 0, D
MACRO_SET_n_r1(0, E);    // SET 0, E
MACRO_SET_n_r1(0, H);    // SET 0, H
MACRO_SET_n_r1(0, L);    // SET 0, L
MACRO_SET_n_r1(0, A);    // SET 0, A

MACRO_SET_n_r1(1, B);    // SET 1, B
MACRO_SET_n_r1(1, C);    // SET 1, C
MACRO_SET_n_r1(1, D);    // SET 1, D
MACRO_SET_n_r1(1, E);    // SET 1, E
MACRO_SET_n_r1(1, H);    // SET 1, H
MACRO_SET_n_r1(1, L);    // SET 1, L
MACRO_SET_n_r1(1, A);    // SET 1, A

MACRO_SET_n_r1(2, B);    // SET 2, B
MACRO_SET_n_r1(2, C);    // SET 2, C
MACRO_SET_n_r1(2, D);    // SET 2, D
MACRO_SET_n_r1(2, E);    // SET 2, E
MACRO_SET_n_r1(2, H);    // SET 2, H
MACRO_SET_n_r1(2, L);    // SET 2, L
MACRO_SET_n_r1(2, A);    // SET 2, A

MACRO_SET_n_r1(3, B);    // SET 3, B
MACRO_SET_n_r1(3, C);    // SET 3, C
MACRO_SET_n_r1(3, D);    // SET 3, D
MACRO_SET_n_r1(3, E);    // SET 3, E
MACRO_SET_n_r1(3, H);    // SET 3, H
MACRO_SET_n_r1(3, L);    // SET 3, L
MACRO_SET_n_r1(3, A);    // SET 3, A

MACRO_SET_n_r1(4, B);    // SET 4, B
MACRO_SET_n_r1(4, C);    // SET 4, C
MACRO_SET_n_r1(4, D);    // SET 4, D
MACRO_SET_n_r1(4, E);    // SET 4, E
MACRO_SET_n_r1(4, H);    // SET 4, H
MACRO_SET_n_r1(4, L);    // SET 4, L
MACRO_SET_n_r1(4, A);    // SET 4, A

MACRO_SET_n_r1(5, B);    // SET 5, B
MACRO_SET_n_r1(5, C);    // SET 5, C
MACRO_SET_n_r1(5, D);    // SET 5, D
MACRO_SET_n_r1(5, E);    // SET 5, E
MACRO_SET_n_r1(5, H);    // SET 5, H
MACRO_SET_n_r1(5, L);    // SET 5, L
MACRO_SET_n_r1(5, A);    // SET 5, A

MACRO_SET_n_r1(6, B);    // SET 6, B
MACRO_SET_n_r1(6, C);    // SET 6, C
MACRO_SET_n_r1(6, D);    // SET 6, D
MACRO_SET_n_r1(6, E);    // SET 6, E
MACRO_SET_n_r1(6, H);    // SET 6, H
MACRO_SET_n_r1(6, L);    // SET 6, L
MACRO_SET_n_r1(6, A);    // SET 6, A

MACRO_SET_n_r1(7, B);    // SET 7, B
MACRO_SET_n_r1(7, C);    // SET 7, C
MACRO_SET_n_r1(7, D);    // SET 7, D
MACRO_SET_n_r1(7, E);    // SET 7, E
MACRO_SET_n_r1(7, H);    // SET 7, H
MACRO_SET_n_r1(7, L);    // SET 7, L
MACRO_SET_n_r1(7, A);    // SET 7, A

#undef MACRO_SET_n_r1

// Macro: SET n, (HL)
#define MACRO_SET_n_HL(n) \
static uint8_t SET_##n##_HL(void) \
{ \
    uint8_t t = mem_read_u8(cpu.reg.HL);\
    t |= (1 << n);\
    mem_write_u8(cpu.reg.HL, t);\
    return 4; \
}

MACRO_SET_n_HL(0);      // SET 0, (HL)
MACRO_SET_n_HL(1);      // SET 1, (HL)
MACRO_SET_n_HL(2);      // SET 2, (HL)
MACRO_SET_n_HL(3);      // SET 3, (HL)
MACRO_SET_n_HL(4);      // SET 4, (HL)
MACRO_SET_n_HL(5);      // SET 5, (HL)
MACRO_SET_n_HL(6);      // SET 6, (HL)
MACRO_SET_n_HL(7);      // SET 7, (HL)

#undef MACRO_SET_n_HL

struct opcode_t opcodeCbList[256] =
{
        {RLC_B,         "RLC B",        1,      true},      // 0x00
        {RLC_C,         "RLC C",        1,      true},      // 0x01
        {RLC_D,         "RLC D",        1,      true},      // 0x02
        {RLC_E,         "RLC E",        1,      true},      // 0x03
        {RLC_H,         "RLC H",        1,      true},      // 0x04
        {RLC_L,         "RLC L",        1,      true},      // 0x05
        {RLC_HL,        "RLC (HL)",     1,      true},      // 0x06
        {RLC_A,         "RLC A",        1,      true},      // 0x07
        {RRC_B,         "RRC B",        1,      true},      // 0x08
        {RRC_C,         "RRC C",        1,      true},      // 0x09
        {RRC_D,         "RRC D",        1,      true},      // 0x0A
        {RRC_E,         "RRC E",        1,      true},      // 0x0B
        {RRC_H,         "RRC H",        1,      true},      // 0x0C
        {RRC_L,         "RRC L",        1,      true},      // 0x0D
        {RRC_HL,        "RRC (HL)",     1,      true},      // 0x0E
        {RRC_A,         "RRC A",        1,      true},      // 0x0F
        {RL_B,          "RL B",         1,      true},      // 0x10
        {RL_C,          "RL C",         1,      true},      // 0x11
        {RL_D,          "RL D",         1,      true},      // 0x12
        {RL_E,          "RL E",         1,      true},      // 0x13
        {RL_H,          "RL H",         1,      true},      // 0x14
        {RL_L,          "RL L",         1,      true},      // 0x15
        {RL_HL,         "RL (HL)",      1,      true},      // 0x16
        {RL_A,          "RL A",         1,      true},      // 0x17
        {RR_B,          "RR B",         1,      true},      // 0x18
        {RR_C,          "RR C",         1,      true},      // 0x19
        {RR_D,          "RR D",         1,      true},      // 0x1A
        {RR_E,          "RR E",         1,      true},      // 0x1B
        {RR_H,          "RR H",         1,      true},      // 0x1C
        {RR_L,          "RR L",         1,      true},      // 0x1D
        {RR_HL,         "RR (HL)",      1,      true},      // 0x1E
        {RR_A,          "RR A",         1,      true},      // 0x1F
        {SLA_B,         "SLA B",        1,      true},      // 0x20
        {SLA_C,         "SLA C",        1,      true},      // 0x21
        {SLA_D,         "SLA D",        1,      true},      // 0x22
        {SLA_E,         "SLA E",        1,      true},      // 0x23
        {SLA_H,         "SLA H",        1,      true},      // 0x24
        {SLA_L,         "SLA L",        1,      true},      // 0x25
        {SLA_HL,        "SLA (HL)",     1,      true},      // 0x26
        {SLA_A,         "SLA A",        1,      true},      // 0x27
        {SRA_B,         "SRA B",        1,      true},      // 0x28
        {SRA_C,         "SRA C",        1,      true},      // 0x29
        {SRA_D,         "SRA D",        1,      true},      // 0x2A
        {SRA_E,         "SRA E",        1,      true},      // 0x2B
        {SRA_H,         "SRA H",        1,      true},      // 0x2C
        {SRA_L,         "SRA L",        1,      true},      // 0x2D
        {SRA_HL,        "SRA (HL)",     1,      true},      // 0x2E
        {SRA_A,         "SRA A",        1,      true},      // 0x2F
        {SWAP_B,        "SWAP B",       1,      true},      // 0x30
        {SWAP_C,        "SWAP C",       1,      true},      // 0x31
        {SWAP_D,        "SWAP D",       1,      true},      // 0x32
        {SWAP_E,        "SWAP E",       1,      true},      // 0x33
        {SWAP_H,        "SWAP H",       1,      true},      // 0x34
        {SWAP_L,        "SWAP L",       1,      true},      // 0x35
        {SWAP_HL,       "SWAP (HL)",    1,      true},      // 0x36
        {SWAP_A,        "SWAP A",       1,      true},      // 0x37
        {SRL_B,         "SRL B",        1,      true},      // 0x38
        {SRL_C,         "SRL C",        1,      true},      // 0x39
        {SRL_D,         "SRL D",        1,      true},      // 0x3A
        {SRL_E,         "SRL E",        1,      true},      // 0x3B
        {SRL_H,         "SRL H",        1,      true},      // 0x3C
        {SRL_L,         "SRL L",        1,      true},      // 0x3D
        {SRL_HL,        "SRL (HL)",     1,      true},      // 0x3E
        {SRL_A,         "SRL A",        1,      true},      // 0x3F
        {BIT_0_B,       "BIT 0,B",      1,      true},      // 0x40
        {BIT_0_C,       "BIT 0,C",      1,      true},      // 0x41
        {BIT_0_D,       "BIT 0,D",      1,      true},      // 0x42
        {BIT_0_E,       "BIT 0,E",      1,      true},      // 0x43
        {BIT_0_H,       "BIT 0,H",      1,      true},      // 0x44
        {BIT_0_L,       "BIT 0,L",      1,      true},      // 0x45
        {BIT_0_HL,      "BIT 0,(HL)",   1,      true},      // 0x46
        {BIT_0_A,       "BIT 0,A",      1,      true},      // 0x47
        {BIT_1_B,       "BIT 1,B",      1,      true},      // 0x48
        {BIT_1_C,       "BIT 1,C",      1,      true},      // 0x49
        {BIT_1_D,       "BIT 1,D",      1,      true},      // 0x4A
        {BIT_1_E,       "BIT 1,E",      1,      true},      // 0x4B
        {BIT_1_H,       "BIT 1,H",      1,      true},      // 0x4C
        {BIT_1_L,       "BIT 1,L",      1,      true},      // 0x4D
        {BIT_1_HL,      "BIT 1,(HL)",   1,      true},      // 0x4E
        {BIT_1_A,       "BIT 1,A",      1,      true},      // 0x4F
        {BIT_2_B,       "BIT 2,B",      1,      true},      // 0x50
        {BIT_2_C,       "BIT 2,C",      1,      true},      // 0x51
        {BIT_2_D,       "BIT 2,D",      1,      true},      // 0x52
        {BIT_2_E,       "BIT 2,E",      1,      true},      // 0x53
        {BIT_2_H,       "BIT 2,H",      1,      true},      // 0x54
        {BIT_2_L,       "BIT 2,L",      1,      true},      // 0x55
        {BIT_2_HL,      "BIT 2,(HL)",   1,      true},      // 0x56
        {BIT_2_A,       "BIT 2,A",      1,      true},      // 0x57
        {BIT_3_B,       "BIT 3,B",      1,      true},      // 0x58
        {BIT_3_C,       "BIT 3,C",      1,      true},      // 0x59
        {BIT_3_D,       "BIT 3,D",      1,      true},      // 0x5A
        {BIT_3_E,       "BIT 3,E",      1,      true},      // 0x5B
        {BIT_3_H,       "BIT 3,H",      1,      true},      // 0x5C
        {BIT_3_L,       "BIT 3,L",      1,      true},      // 0x5D
        {BIT_3_HL,      "BIT 3,(HL)",   1,      true},      // 0x5E
        {BIT_3_A,       "BIT 3,A",      1,      true},      // 0x5F
        {BIT_4_B,       "BIT 4,B",      1,      true},      // 0x60
        {BIT_4_C,       "BIT 4,C",      1,      true},      // 0x61
        {BIT_4_D,       "BIT 4,D",      1,      true},      // 0x62
        {BIT_4_E,       "BIT 4,E",      1,      true},      // 0x63
        {BIT_4_H,       "BIT 4,H",      1,      true},      // 0x64
        {BIT_4_L,       "BIT 4,L",      1,      true},      // 0x65
        {BIT_4_HL,      "BIT 4,(HL)",   1,      true},      // 0x66
        {BIT_4_A,       "BIT 4,A",      1,      true},      // 0x67
        {BIT_5_B,       "BIT 5,B",      1,      true},      // 0x68
        {BIT_5_C,       "BIT 5,C",      1,      true},      // 0x69
        {BIT_5_D,       "BIT 5,D",      1,      true},      // 0x6A
        {BIT_5_E,       "BIT 5,E",      1,      true},      // 0x6B
        {BIT_5_H,       "BIT 5,H",      1,      true},      // 0x6C
        {BIT_5_L,       "BIT 5,L",      1,      true},      // 0x6D
        {BIT_5_HL,      "BIT 5,(HL)",   1,      true},      // 0x6E
        {BIT_5_A,       "BIT 5,A",      1,      true},      // 0x6F
        {BIT_6_B,       "BIT 6,B",      1,      true},      // 0x70
        {BIT_6_C,       "BIT 6,C",      1,      true},      // 0x71
        {BIT_6_D,       "BIT 6,D",      1,      true},      // 0x72
        {BIT_6_E,       "BIT 6,E",      1,      true},      // 0x73
        {BIT_6_H,       "BIT 6,H",      1,      true},      // 0x74
        {BIT_6_L,       "BIT 6,L",      1,      true},      // 0x75
        {BIT_6_HL,      "BIT 6,(HL)",   1,      true},      // 0x76
        {BIT_6_A,       "BIT 6,A",      1,      true},      // 0x77
        {BIT_7_B,       "BIT 7,B",      1,      true},      // 0x78
        {BIT_7_C,       "BIT 7,C",      1,      true},      // 0x79
        {BIT_7_D,       "BIT 7,D",      1,      true},      // 0x7A
        {BIT_7_E,       "BIT 7,E",      1,      true},      // 0x7B
        {BIT_7_H,       "BIT 7,H",      1,      true},      // 0x7C
        {BIT_7_L,       "BIT 7,L",      1,      true},      // 0x7D
        {BIT_7_HL,      "BIT 7,(HL)",   1,      true},      // 0x7E
        {BIT_7_A,       "BIT 7,A",      1,      true},      // 0x7F
        {RES_0_B,       "RES 0,B",      1,      true},      // 0x80
        {RES_0_C,       "RES 0,C",      1,      true},      // 0x81
        {RES_0_D,       "RES 0,D",      1,      true},      // 0x82
        {RES_0_E,       "RES 0,E",      1,      true},      // 0x83
        {RES_0_H,       "RES 0,H",      1,      true},      // 0x84
        {RES_0_L,       "RES 0,L",      1,      true},      // 0x85
        {RES_0_HL,      "RES 0,(HL)",   1,      true},      // 0x86
        {RES_0_A,       "RES 0,A",      1,      true},      // 0x87
        {RES_1_B,       "RES 1,B",      1,      true},      // 0x88
        {RES_1_C,       "RES 1,C",      1,      true},      // 0x89
        {RES_1_D,       "RES 1,D",      1,      true},      // 0x8A
        {RES_1_E,       "RES 1,E",      1,      true},      // 0x8B
        {RES_1_H,       "RES 1,H",      1,      true},      // 0x8C
        {RES_1_L,       "RES 1,L",      1,      true},      // 0x8D
        {RES_1_HL,      "RES 1,(HL)",   1,      true},      // 0x8E
        {RES_1_A,       "RES 1,A",      1,      true},      // 0x8F
        {RES_2_B,       "RES 2,B",      1,      true},      // 0x90
        {RES_2_C,       "RES 2,C",      1,      true},      // 0x91
        {RES_2_D,       "RES 2,D",      1,      true},      // 0x92
        {RES_2_E,       "RES 2,E",      1,      true},      // 0x93
        {RES_2_H,       "RES 2,H",      1,      true},      // 0x94
        {RES_2_L,       "RES 2,L",      1,      true},      // 0x95
        {RES_2_HL,      "RES 2,(HL)",   1,      true},      // 0x96
        {RES_2_A,       "RES 2,A",      1,      true},      // 0x97
        {RES_3_B,       "RES 3,B",      1,      true},      // 0x98
        {RES_3_C,       "RES 3,C",      1,      true},      // 0x99
        {RES_3_D,       "RES 3,D",      1,      true},      // 0x9A
        {RES_3_E,       "RES 3,E",      1,      true},      // 0x9B
        {RES_3_H,       "RES 3,H",      1,      true},      // 0x9C
        {RES_3_L,       "RES 3,L",      1,      true},      // 0x9D
        {RES_3_HL,      "RES 3,(HL)",   1,      true},      // 0x9E
        {RES_3_A,       "RES 3,A",      1,      true},      // 0x9F
        {RES_4_B,       "RES 4,B",      1,      true},      // 0xA0
        {RES_4_C,       "RES 4,C",      1,      true},      // 0xA1
        {RES_4_D,       "RES 4,D",      1,      true},      // 0xA2
        {RES_4_E,       "RES 4,E",      1,      true},      // 0xA3
        {RES_4_H,       "RES 4,H",      1,      true},      // 0xA4
        {RES_4_L,       "RES 4,L",      1,      true},      // 0xA5
        {RES_4_HL,      "RES 4,(HL)",   1,      true},      // 0xA6
        {RES_4_A,       "RES 4,A",      1,      true},      // 0xA7
        {RES_5_B,       "RES 5,B",      1,      true},      // 0xA8
        {RES_5_C,       "RES 5,C",      1,      true},      // 0xA9
        {RES_5_D,       "RES 5,D",      1,      true},      // 0xAA
        {RES_5_E,       "RES 5,E",      1,      true},      // 0xAB
        {RES_5_H,       "RES 5,H",      1,      true},      // 0xAC
        {RES_5_L,       "RES 5,L",      1,      true},      // 0xAD
        {RES_5_HL,      "RES 5,(HL)",   1,      true},      // 0xAE
        {RES_5_A,       "RES 5,A",      1,      true},      // 0xAF
        {RES_6_B,       "RES 6,B",      1,      true},      // 0xB0
        {RES_6_C,       "RES 6,C",      1,      true},      // 0xB1
        {RES_6_D,       "RES 6,D",      1,      true},      // 0xB2
        {RES_6_E,       "RES 6,E",      1,      true},      // 0xB3
        {RES_6_H,       "RES 6,H",      1,      true},      // 0xB4
        {RES_6_L,       "RES 6,L",      1,      true},      // 0xB5
        {RES_6_HL,      "RES 6,(HL)",   1,      true},      // 0xB6
        {RES_6_A,       "RES 6,A",      1,      true},      // 0xB7
        {RES_7_B,       "RES 7,B",      1,      true},      // 0xB8
        {RES_7_C,       "RES 7,C",      1,      true},      // 0xB9
        {RES_7_D,       "RES 7,D",      1,      true},      // 0xBA
        {RES_7_E,       "RES 7,E",      1,      true},      // 0xBB
        {RES_7_H,       "RES 7,H",      1,      true},      // 0xBC
        {RES_7_L,       "RES 7,L",      1,      true},      // 0xBD
        {RES_7_HL,      "RES 7,(HL)",   1,      true},      // 0xBE
        {RES_7_A,       "RES 7,A",      1,      true},      // 0xBF
        {SET_0_B,       "SET 0,B",      1,      true},      // 0xC0
        {SET_0_C,       "SET 0,C",      1,      true},      // 0xC1
        {SET_0_D,       "SET 0,D",      1,      true},      // 0xC2
        {SET_0_E,       "SET 0,E",      1,      true},      // 0xC3
        {SET_0_H,       "SET 0,H",      1,      true},      // 0xC4
        {SET_0_L,       "SET 0,L",      1,      true},      // 0xC5
        {SET_0_HL,      "SET 0,(HL)",   1,      true},      // 0xC6
        {SET_0_A,       "SET 0,A",      1,      true},      // 0xC7
        {SET_1_B,       "SET 1,B",      1,      true},      // 0xC8
        {SET_1_C,       "SET 1,C",      1,      true},      // 0xC9
        {SET_1_D,       "SET 1,D",      1,      true},      // 0xCA
        {SET_1_E,       "SET 1,E",      1,      true},      // 0xCB
        {SET_1_H,       "SET 1,H",      1,      true},      // 0xCC
        {SET_1_L,       "SET 1,L",      1,      true},      // 0xCD
        {SET_1_HL,      "SET 1,(HL)",   1,      true},      // 0xCE
        {SET_1_A,       "SET 1,A",      1,      true},      // 0xCF
        {SET_2_B,       "SET 2,B",      1,      true},      // 0xD0
        {SET_2_C,       "SET 2,C",      1,      true},      // 0xD1
        {SET_2_D,       "SET 2,D",      1,      true},      // 0xD2
        {SET_2_E,       "SET 2,E",      1,      true},      // 0xD3
        {SET_2_H,       "SET 2,H",      1,      true},      // 0xD4
        {SET_2_L,       "SET 2,L",      1,      true},      // 0xD5
        {SET_2_HL,      "SET 2,(HL)",   1,      true},      // 0xD6
        {SET_2_A,       "SET 2,A",      1,      true},      // 0xD7
        {SET_3_B,       "SET 3,B",      1,      true},      // 0xD8
        {SET_3_C,       "SET 3,C",      1,      true},      // 0xD9
        {SET_3_D,       "SET 3,D",      1,      true},      // 0xDA
        {SET_3_E,       "SET 3,E",      1,      true},      // 0xDB
        {SET_3_H,       "SET 3,H",      1,      true},      // 0xDC
        {SET_3_L,       "SET 3,L",      1,      true},      // 0xDD
        {SET_3_HL,      "SET 3,(HL)",   1,      true},      // 0xDE
        {SET_3_A,       "SET 3,A",      1,      true},      // 0xDF
        {SET_4_B,       "SET 4,B",      1,      true},      // 0xE0
        {SET_4_C,       "SET 4,C",      1,      true},      // 0xE1
        {SET_4_D,       "SET 4,D",      1,      true},      // 0xE2
        {SET_4_E,       "SET 4,E",      1,      true},      // 0xE3
        {SET_4_H,       "SET 4,H",      1,      true},      // 0xE4
        {SET_4_L,       "SET 4,L",      1,      true},      // 0xE5
        {SET_4_HL,      "SET 4,(HL)",   1,      true},      // 0xE6
        {SET_4_A,       "SET 4,A",      1,      true},      // 0xE7
        {SET_5_B,       "SET 5,B",      1,      true},      // 0xE8
        {SET_5_C,       "SET 5,C",      1,      true},      // 0xE9
        {SET_5_D,       "SET 5,D",      1,      true},      // 0xEA
        {SET_5_E,       "SET 5,E",      1,      true},      // 0xEB
        {SET_5_H,       "SET 5,H",      1,      true},      // 0xEC
        {SET_5_L,       "SET 5,L",      1,      true},      // 0xED
        {SET_5_HL,      "SET 5,(HL)",   1,      true},      // 0xEE
        {SET_5_A,       "SET 5,A",      1,      true},      // 0xEF
        {SET_6_B,       "SET 6,B",      1,      true},      // 0xF0
        {SET_6_C,       "SET 6,C",      1,      true},      // 0xF1
        {SET_6_D,       "SET 6,D",      1,      true},      // 0xF2
        {SET_6_E,       "SET 6,E",      1,      true},      // 0xF3
        {SET_6_H,       "SET 6,H",      1,      true},      // 0xF4
        {SET_6_L,       "SET 6,L",      1,      true},      // 0xF5
        {SET_6_HL,      "SET 6,(HL)",   1,      true},      // 0xF6
        {SET_6_A,       "SET 6,A",      1,      true},      // 0xF7
        {SET_7_B,       "SET 7,B",      1,      true},      // 0xF8
        {SET_7_C,       "SET 7,C",      1,      true},      // 0xF9
        {SET_7_D,       "SET 7,D",      1,      true},      // 0xFA
        {SET_7_E,       "SET 7,E",      1,      true},      // 0xFB
        {SET_7_H,       "SET 7,H",      1,      true},      // 0xFC
        {SET_7_L,       "SET 7,L",      1,      true},      // 0xFD
        {SET_7_HL,      "SET 7,(HL)",   1,      true},      // 0xFE
        {SET_7_A,       "SET 7,A",      1,      true},      // 0xFF
};
