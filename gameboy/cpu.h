/*
 * cpu.h
 *
 *  Created on: 6 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_CPU_H_
#define INC_GAMEBOY_CPU_H_

#include <stdint.h>
#include <stdbool.h>

struct cpu_reg_t
{
	union
	{
		struct
		{
			union
			{
				uint8_t F; // Flags
				struct
				{
					uint8_t  : 4;
					uint8_t C: 1; // Carry Flag
					uint8_t H: 1; // Half Carry Flag
					uint8_t N: 1; // Subtract Flag
					uint8_t Z: 1; // Zero Flag
				} Flags;
			};
			uint8_t A; // Accumulator
		};
		uint16_t AF;
	};

	union
	{
		struct
		{
			uint8_t C;
			uint8_t B;
		};
		uint16_t BC;
	};

	union
	{
		struct
		{
			uint8_t E;
			uint8_t D;
		};
		uint16_t DE;
	};

	union
	{
		struct
		{
			uint8_t L;
			uint8_t H;
		};
		uint16_t HL;
	};
	uint16_t SP; // Stack Pointer
	uint16_t PC; // Program Counter
} __attribute__((__packed__));

struct cpu_t
{
    struct cpu_reg_t reg;
    bool halted;
    uint8_t cycle_counter;
    bool prefix_cb;
};

extern struct cpu_t cpu;

void cpu_init(void);
void cpu_exec(bool PrintState);

#endif /* INC_GAMEBOY_CPU_H_ */
