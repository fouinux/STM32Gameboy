/*
 * apu.c
 *
 *  Created on: 23 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#include "apu.h"
#include "mem.h"

struct apu_t apu;

void apu_init(void)
{
    apu.pReg = (struct apu_reg_t *) mem_get_register(SOUND);
}