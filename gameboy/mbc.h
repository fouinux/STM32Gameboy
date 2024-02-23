/*
 * mbc.h
 *
 *  Created on: 24 feb. 2024
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_MBC_H_
#define INC_GAMEBOY_MBC_H_

#include <stdint.h>

typedef void (*mbc_func_t)(uint16_t, uint8_t);

mbc_func_t mbc_get_callback(uint8_t Code);

#endif /* INC_GAMEBOY_MBC_H_ */
