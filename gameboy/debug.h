/*
 * debug.h
 *
 *  Created on: 31 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_DEBUG_H_
#define INC_GAMEBOY_DEBUG_H_

#include <stdio.h>
#include <time.h>

static inline void debug_print_clock_diff(const char* pString, struct timespec* pStart, struct timespec* pStop)
{
    struct timespec timeDiff;

    if (pStop->tv_nsec < pStart->tv_nsec)
    {
        timeDiff.tv_sec = pStop->tv_sec - pStart->tv_sec - 1;
        timeDiff.tv_nsec = pStop->tv_nsec - pStart->tv_nsec + 1000000000LL;
    }
    else
    {
        timeDiff.tv_sec = pStop->tv_sec - pStart->tv_sec;
        timeDiff.tv_nsec = pStop->tv_nsec - pStart->tv_nsec;
    }

    printf("%s: %ld\n", pString, timeDiff.tv_nsec);
}

#endif /* INC_GAMEBOY_DEBUG_H_ */