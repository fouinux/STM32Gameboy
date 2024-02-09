/*
 * fifo.h
 *
 *  Created on: 22 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_FIFO_H_
#define INC_GAMEBOY_FIFO_H_

#include <stdint.h>

#define FIFO_LENGTH     16

struct fifo_t
{
    uint8_t aData[FIFO_LENGTH];
    uint8_t Read;
    uint8_t Write;
    uint8_t Size;
};

void fifo_init(struct fifo_t *pFifo);
void fifo_enqueue(struct fifo_t *pFifo, uint8_t Data);
uint8_t fifo_dequeue(struct fifo_t *pFifo);
void fifo_flush(struct fifo_t *pFifo);

#endif /* INC_GAMEBOY_FIFO_H_ */
