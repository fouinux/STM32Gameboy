/*
 * fifo.c
 *
 *  Created on: 22 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#include "fifo.h"

#include <stdio.h>

void fifo_init(struct fifo_t *pFifo)
{
    if (pFifo == NULL)
        return;

    pFifo->Read = 0;
    pFifo->Write = 0;
    pFifo->Size = 0;
}

void fifo_enqueue(struct fifo_t *pFifo, uint8_t Data)
{
    if (pFifo == NULL)
        return;

    if (pFifo->Size < FIFO_LENGTH)
    {
        pFifo->aData[pFifo->Write++] = Data;
        if (pFifo->Write == FIFO_LENGTH)
            pFifo->Write = 0;

        pFifo->Size++;
    }
}

uint8_t fifo_dequeue(struct fifo_t *pFifo)
{
    if (pFifo == NULL)
        return 0;

    uint8_t data = 0;
    if (pFifo->Size > 0)
    {
        data = pFifo->aData[pFifo->Read++];
        if (pFifo->Read == FIFO_LENGTH)
            pFifo->Read = 0;

        pFifo->Size--;
    }
    return data;
}

void fifo_flush(struct fifo_t *pFifo)
{
    if (pFifo != NULL)
    {
        pFifo->Read = 0;
        pFifo->Write = 0;
        pFifo->Size = 0;

    }
}
