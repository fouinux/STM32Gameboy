/*
 * core.c
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/core.h>
#include <gameboy/memory.h>
#include <gameboy/opcode.h>
#include <gameboy/opcode_cb.h>

// Also defined as global
struct core_t core;

void core_init(void)
{
    // Init ALU
    core.reg.AF = 0;
    core.reg.BC = 0;
    core.reg.DE = 0;
    core.reg.HL = 0;
    core.reg.SP = 0;
    core.reg.PC = 0;

    // Init Flags
    core.ime = false;
    core.halted = false;
    core.cycle_counter = 1;
}

void core_execute(void)
{
    bool update_pc = false;

    core.cycle_counter--;

    if (0 == core.cycle_counter)
    {
        // Read opcode
        uint8_t opcode = mem_read_u8(core.reg.PC);

        // Execute opcode
        if (core.prefix_cb)
        {
            core.prefix_cb = false;
            core.cycle_counter = opcodeCbList[opcode].func();
            update_pc = opcodeCbList[opcode].update_pc;
        }
        else
        {
            core.cycle_counter = opcodeList[opcode].func();
            update_pc = opcodeList[opcode].update_pc;
        }

        // Update Program Counter
        if (update_pc)
            core.reg.PC += opcodeList[opcode].length;
    }
}
