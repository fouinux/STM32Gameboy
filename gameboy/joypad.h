/*
 * joypad.h
 *
 *  Created on: 24 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_JOYPAD_H_
#define INC_GAMEBOY_JOYPAD_H_

#include <stdint.h>
#include <stdbool.h>

struct joypad_reg_t
{
    union
    {
        uint8_t P1;
        struct
        {
            union
            {
                struct
                {
                    uint8_t A : 1;
                    uint8_t B : 1;
                    uint8_t Select : 1;
                    uint8_t Start : 1;
                    uint8_t SelectDPad : 1;
                    uint8_t SelectButtons : 1;
                    uint8_t : 1;
                };

                struct
                {
                    uint8_t Right : 1;
                    uint8_t Left : 1;
                    uint8_t Up : 1;
                    uint8_t Down : 1;
                    uint8_t : 4;
                };
            };
        };
    };
} __attribute__ ((__packed__));

struct joypad_t
{
    struct joypad_reg_t *pReg;

    // Internal input states
    uint8_t DPad;
    uint8_t Buttons;
};

extern struct joypad_t joypad;

enum joypad_input
{
    A,
    B,
    SELECT,
    START,
    RIGHT,
    LEFT,
    UP,
    DOWN
};

void joypad_init(void);
void joypad_set_input(enum joypad_input Input, bool State);
void joypad_update(void);

#endif /* INC_GAMEBOY_JOYPAD_H_ */