/*
 * opcode_ll.h
 *
 *  Created on: 7 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_CPU_OPCODE_LL_H_
#define INC_GAMEBOY_CPU_OPCODE_LL_H_

#include <stdint.h>

void ll_RLC(uint8_t *pReg);
void ll_RL(uint8_t *pReg);
void ll_RRC(uint8_t *pReg);
void ll_RR(uint8_t *pReg);

#endif /* INC_GAMEBOY_CPU_OPCODE_LL_H_ */
