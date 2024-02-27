/*
 * irq.c
 *
 *  Created on: 18 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include "irq.h"
#include "cpu.h"
#include "mem.h"

#define IRQ_MASK_VBLANK     0x01
#define IRQ_MASK_LCDC       0x02
#define IRQ_MASK_TIMER      0x04
#define IRQ_MASK_SERIAL     0x08
#define IRQ_MASK_P10_P13    0x80

#define IRQ_ADDR_VBLANK     0x40
#define IRQ_ADDR_LCDC       0x48
#define IRQ_ADDR_TIMER      0x50
#define IRQ_ADDR_SERIAL     0x58
#define IRQ_ADDR_P10_P13    0x60

#define IRQ_SWITCH_CYCLE    4

// Exported to be use directly
struct irq_t irq;

inline static void switch_context(uint16_t Addr)
{
    mem_write_u16(cpu.reg.SP - 2, cpu.reg.PC);
    cpu.reg.SP -= 2;
    cpu.reg.PC = Addr;
}

void irq_init(void)
{
    irq.ime = false;
    irq.pIF = (struct irq_reg_t *) mem_get_register(IF);
    irq.pIF->Value = 0x00;
    irq.pIE = (struct irq_reg_t *) mem_get_register(IE);
    irq.pIE->Value = 0x00;
}

bool irq_check(void)
{
    if (true == irq.ime)
    {
        uint8_t mask = irq.pIE->Value & irq.pIF->Value;
        if (mask)
        {
            // Disable IRQ
            irq.ime = false;

            printf("IRQ: %02x\n", mask);

            // Search for active IRQ according to priorities
            if (mask & IRQ_MASK_VBLANK) // V-Blank
            {
                irq.pIF->Flags.VBlank = 0;
                switch_context(IRQ_ADDR_VBLANK);
                cpu.cycle_counter = IRQ_SWITCH_CYCLE;
                return true;
            }

            if (mask & IRQ_MASK_LCDC) // LCDC
            {
                irq.pIF->Flags.LCDC = 0;
                switch_context(IRQ_ADDR_LCDC);
                cpu.cycle_counter = IRQ_SWITCH_CYCLE;
                return true;
            }

            if (mask & IRQ_MASK_TIMER) // Timer
            {
                irq.pIF->Flags.Timer = 0;
                switch_context(IRQ_ADDR_TIMER);
                cpu.cycle_counter = IRQ_SWITCH_CYCLE;
                return true;
            }

            if (mask & IRQ_MASK_SERIAL) // Serial
            {
                irq.pIF->Flags.Serial = 0;
                switch_context(IRQ_ADDR_SERIAL);
                cpu.cycle_counter = IRQ_SWITCH_CYCLE;
                return true;
            }

            if (mask & IRQ_MASK_P10_P13) // P10_P13
            {
                irq.pIF->Flags.P10_P13 = 0;
                switch_context(IRQ_ADDR_P10_P13);
                cpu.cycle_counter = IRQ_SWITCH_CYCLE;
                return true;
            }
        }
        return false;
    }
    else
        return false;
}

bool irq_pending(void)
{
    if ((irq.pIE->Value & irq.pIF->Value) != 0x00)
        return true;
    return false;
}
