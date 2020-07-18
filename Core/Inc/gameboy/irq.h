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
        uint8_t IF; // 0xFF0F - Interrupt Flag
        struct
        {
            uint8_t : 3;
            uint8_t P10_P13 : 1;
            uint8_t Serial : 1;
            uint8_t Timer : 1;
            uint8_t LCDC : 1;
            uint8_t VBlank : 1;
        } IF_Flags;
    };

    union
    {
        uint8_t IE; // 0xFFFF - Interrupt Enable
        struct
        {
            uint8_t : 3;
            uint8_t P10_P13 : 1;
            uint8_t Serial : 1;
            uint8_t Timer : 1;
            uint8_t LCDC : 1;
            uint8_t VBlank : 1;
        } IE_Flags;
    };
};

struct irq_t
{
    struct irq_reg_t reg;
    bool ime; // Interrupt Master Enable
};

extern struct irq_t irq;

void irq_init(void);
bool irq_check(void);

#endif /* INC_GAMEBOY_IRQ_H_ */
