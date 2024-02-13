/*
 * serial.c
 *
 *  Created on: 13 feb. 2024
 *      Author: Guillaume Fouilleul
 */

#include "serial.h"
#include "mem.h"
#include "irq.h"

struct serial_t serial;

void serial_init(void)
{
    serial.pReg = (struct serial_reg_t *) mem_get_register(SERIAL);

    serial.clock_cnt = 0;
    serial.xfer_cnt = 0;
    serial.output_buffer = 0;
}

void serial_exec(void)
{
    serial.clock_cnt++;

    if ((serial.clock_cnt & 0x7F) == 0x00)
    {
        if (serial.pReg->SC_Flags.TransferEnable == 1)
        {
            // Output buffer
            serial.output_buffer <<= 1;
            if (serial.pReg->SB & 0x80)
                serial.output_buffer++;

            serial.pReg->SB <<= 1;
            serial.xfer_cnt++;
            if (serial.xfer_cnt == 8)
            {
                serial.pReg->SC_Flags.TransferEnable = 0;
                serial.xfer_cnt = 0;
                irq.pIF->Flags.Serial = 1;

                printf("%c\n", serial.output_buffer);
                // putchar(serial.output_buffer);
            }
        }
    }
}
