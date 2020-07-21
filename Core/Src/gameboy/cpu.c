/*
 * cpu.c
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/cpu.h>
#include <gameboy/irq.h>
#include <gameboy/mem.h>
#include <gameboy/opcode.h>
#include <gameboy/opcode_cb.h>

// Exported to be use directly
struct cpu_t cpu;

void cpu_init(void)
{
    // Init ALU
    cpu.reg.AF = 0;
    cpu.reg.BC = 0;
    cpu.reg.DE = 0;
    cpu.reg.HL = 0;
    cpu.reg.SP = 0;
    cpu.reg.PC = 0;

    // Init Flags
    cpu.halted = false;
    cpu.cycle_counter = 1;
}

void cpu_exec(void)
{
    bool update_pc = false;

    cpu.cycle_counter--;

    if (0 == cpu.cycle_counter)
    {
        // Check for interrupt
        if (false == irq_check())
        {
            // Read opcode
            uint8_t opcode = mem_read_u8(cpu.reg.PC);

            // Execute opcode
            if (cpu.prefix_cb)
            {
                cpu.prefix_cb = false;
                cpu.cycle_counter = opcodeCbList[opcode].func();
                update_pc = opcodeCbList[opcode].update_pc;
            }
            else
            {
                cpu.cycle_counter = opcodeList[opcode].func();
                update_pc = opcodeList[opcode].update_pc;
            }

            // Update Program Counter
            if (update_pc)
                cpu.reg.PC += opcodeList[opcode].length;
        }
    }
}
