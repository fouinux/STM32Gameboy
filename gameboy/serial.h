/*
 * serial.h
 *
 *  Created on: 13 feb. 2024
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_SERIAL_H_
#define INC_GAMEBOY_SERIAL_H_

#include <stdint.h>

struct serial_reg_t
{
    uint8_t SB;
    union
    {
        uint8_t SC;
        struct
        {
            uint8_t ClockSelect : 1;
            uint8_t : 6;
            uint8_t TransferEnable : 1;
        } SC_Flags;
    };
};

struct serial_t
{
    struct serial_reg_t *pReg;
    uint8_t clock_cnt; // Used to divide main clock to get 8192 Hz
    uint8_t xfer_cnt;
    uint8_t output_buffer;
};

void serial_init(void);
void serial_exec(void);

#endif /* INC_GAMEBOY_SERIAL_H_ */