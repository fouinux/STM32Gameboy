/*
 * debug.c
 *
 *  Created on: 31 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#include "debug.h"

struct debug_t debug;

void debug_init(void)
{
    debug.cpu = false;
}
