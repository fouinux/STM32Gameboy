/*
 * joypad.c
 *
 *  Created on: 24 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#include "joypad.h"
#include "mem.h"
#include "irq.h"

#define JOYPAD_BUTTON_P10_MASK_SET        0x01
#define JOYPAD_BUTTON_P10_MASK_RESET      (~JOYPAD_BUTTON_P10_MASK_SET)
#define JOYPAD_BUTTON_P11_MASK_SET        0x02
#define JOYPAD_BUTTON_P11_MASK_RESET      (~JOYPAD_BUTTON_P11_MASK_SET)
#define JOYPAD_BUTTON_P12_MASK_SET        0x04
#define JOYPAD_BUTTON_P12_MASK_RESET      (~JOYPAD_BUTTON_P12_MASK_SET)
#define JOYPAD_BUTTON_P13_MASK_SET        0x08
#define JOYPAD_BUTTON_P13_MASK_RESET      (~JOYPAD_BUTTON_P13_MASK_SET)

struct joypad_t joypad;

void joypad_init(void)
{
    joypad.pReg = (struct joypad_reg_t*) mem_get_register(JOYPAD);

    joypad.Buttons = 0x00;
    joypad.DPad = 0x00;
}

void joypad_set_input(enum joypad_input Input, bool State)
{
    switch (Input)
    {
        case A:
            if (State)
                joypad.Buttons |= JOYPAD_BUTTON_P10_MASK_SET;
            else
                joypad.Buttons &= JOYPAD_BUTTON_P10_MASK_RESET;
            break;
        case B:
            if (State)
                joypad.Buttons |= JOYPAD_BUTTON_P11_MASK_SET;
            else
                joypad.Buttons &= JOYPAD_BUTTON_P11_MASK_RESET;
            break;
        case SELECT:
            if (State)
                joypad.Buttons |= JOYPAD_BUTTON_P12_MASK_SET;
            else
                joypad.Buttons &= JOYPAD_BUTTON_P12_MASK_RESET;
            break;
        case START:
            if (State)
                joypad.Buttons |= JOYPAD_BUTTON_P13_MASK_SET;
            else
                joypad.Buttons &= JOYPAD_BUTTON_P13_MASK_RESET;
            break;
        case RIGHT:
            if (State)
                joypad.DPad |= JOYPAD_BUTTON_P10_MASK_SET;
            else
                joypad.DPad &= JOYPAD_BUTTON_P10_MASK_RESET;
            break;
        case LEFT:
            if (State)
                joypad.DPad |= JOYPAD_BUTTON_P11_MASK_SET;
            else
                joypad.DPad &= JOYPAD_BUTTON_P11_MASK_RESET;
            break;
        case UP:
            if (State)
                joypad.DPad |= JOYPAD_BUTTON_P12_MASK_SET;
            else
                joypad.DPad &= JOYPAD_BUTTON_P12_MASK_RESET;
            break;
        case DOWN:
            if (State)
                joypad.DPad |= JOYPAD_BUTTON_P13_MASK_SET;
            else
                joypad.DPad &= JOYPAD_BUTTON_P13_MASK_RESET;
            break;
    }

    joypad_update();
}

void joypad_update(void)
{
    uint8_t nibble = 0x00;

    if (joypad.pReg->SelectDPad == 0)
        nibble = joypad.DPad;
    if (joypad.pReg->SelectButtons == 0)
        nibble |= joypad.Buttons;

    // IRQ: Falling edge detection on P10-P13
    uint8_t last = (~joypad.pReg->P1) & 0x0F;
    if (last & nibble)
    {
        irq.pIF->Flags.P10_P13 = 1;
    }

    nibble ^= 0x0F; // Invert bits

    joypad.pReg->P1 = (joypad.pReg->P1 & 0x30) | (nibble & 0x0F);
}
