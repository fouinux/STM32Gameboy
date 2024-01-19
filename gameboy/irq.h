/*
 * irq.h
 *
 *  Created on: 18 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_IRQ_H_
#define INC_GAMEBOY_IRQ_H_

#include <stdint.h>
#include <stdbool.h>

struct irq_reg_t
{
    union
    {
        uint8_t Value;
        struct
        {
            uint8_t VBlank : 1;
            uint8_t LCDC : 1;
            uint8_t Timer : 1;
            uint8_t Serial : 1;
            uint8_t P10_P13 : 1;
            uint8_t : 3;
        } Flags;
    };
};

struct irq_t
{
    struct irq_reg_t *pIF; // 0xFF0F - Interrupt Flag
    struct irq_reg_t *pIE; // 0xFFFF - Interrupt Enable
    bool ime; // Interrupt Master Enable
};

extern struct irq_t irq;

void irq_init(void);
bool irq_check(void);

#endif /* INC_GAMEBOY_IRQ_H_ */
