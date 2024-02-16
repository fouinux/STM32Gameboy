/*
 * timer.c
 *
 *  Created on: 05 juil. 2022
 *      Author: Guillaume Fouilleul
 */

#include "timer.h"
#include "mem.h"
#include "irq.h"

struct timer_t timer;

void timer_init(void)
{
    timer.pReg = (struct timer_reg_t *) mem_get_register(TIMER);
}

/**
 * This function needs to be run at the same rate as the CPU cycle.
 */
void timer_exec(void)
{
    // Run DIV register
    timer.fullDiv += 4;
    timer.pReg->DIV = (timer.fullDiv >> 8) & 0xFF;

    // Timer is running ?
    if (timer.pReg->TAC_Flags.TimerStart)
    {
        uint8_t aClockShift[] = {9, 3, 5, 7};
        uint8_t clockShift = aClockShift[timer.pReg->TAC_Flags.InputClockSelect];
        bool currentBit = (timer.fullDiv >> clockShift) & 0x01;

        if (currentBit == false && timer.lastBit == true)  // Falling Edge
        {
            if (timer.pReg->TIMA == 0xFF) // Overflow
            {
                // Copy TIM in TIMA
                timer.pReg->TIMA = timer.pReg->TIM;

                // Generate irq
                irq.pIF->Flags.Timer = 1;
            }
            else
            {
                timer.pReg->TIMA++;
            }
        }

        timer.lastBit = currentBit;
    }
}
