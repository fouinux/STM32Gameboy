/*
 * timer.h
 *
 *  Created on: 05 juil. 2022
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_TIMER_H_
#define INC_GAMEBOY_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

struct timer_reg_t
{
    uint8_t DIV;  // 0xFF04 - Divider register (R/W)
    uint8_t TIMA; // 0xFF05 - Timer Counter (R/W)
    uint8_t TIM;  // 0xFF06 - Timer Modulo (R/W)

    union
    {
        uint8_t TAC;  // 0xFF07 - Timer Control (R/W)
        struct
        {
            uint8_t InputClockSelect : 2;
            uint8_t TimerStart : 1;
            uint8_t : 5;
        } TAC_Flags;
    };

} __attribute__ ((__packed__));

struct timer_t
{
    struct timer_reg_t *pReg;
    uint16_t fullDiv;  // 16 bits resolution of DIV register
    bool lastBit; // Used for falling edge detection
};

void timer_init(void);
void timer_exec(void);

#endif /* INC_GAMEBOY_TIMER_H_ */
