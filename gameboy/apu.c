/*
 * apu.c
 *
 *  Created on: 23 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#include "apu.h"
#include "mem.h"
#include "timer.h"

#include <stdint.h>

enum env_dir
{
    DECREASE = 0,
    INCREASE
};

enum lfsr_width
{
    LFSR_15_BITS = 0,
    LFSR_7_BITS
};

struct apu_t apu;

void apu_init(void)
{
    apu.pReg = (struct apu_reg_t *) mem_get_register(SOUND);

    apu.div_apu_cnt = 0;

    // Channel 4
    apu.ch4.enabled = false;
    apu.ch4.len_timer_en = false;
    apu.ch4.len_timer = 0;
    apu.ch4.lfsr = 0;
}

void apu_exec(void)
{
    // DIV_APU
    if (((apu.div_apu & 0x10) == 0x10) && ((timer.pReg->DIV & 0x10) == 0x00))
    {
        apu.div_apu_cnt++;

        if (apu.div_apu_cnt % 2 == 0)
        {
            // Sound length
            if (apu.ch4.enabled && apu.ch4.len_timer_en)
            {
                apu.ch4.len_timer--;
                if (apu.ch4.len_timer == 0)
                    apu.ch4.enabled = false;
            }
            
        }

        if (apu.div_apu_cnt % 4 == 0)
        {
            // CH1 freq sweep
        }

        if (apu.div_apu_cnt % 8 == 0)
        {
            // Envelope sweep
        }
    }
    apu.div_apu = timer.pReg->DIV;

    // Channel 4 : Noise
}

void apu_update_nr44(void)
{
    if (apu.pReg->NR44_Flags.Trigger)
    {
        // Length Timer
        if (apu.pReg->NR44_Flags.LengthEnable)
        {
            apu.ch4.len_timer_en = true;
            apu.ch4.len_timer = apu.pReg->NR41_Flags.Length;
        }
        else
            apu.ch4.len_timer_en = false;

        // Envelope Sweep
        if (apu.pReg->NR42_Flags.SweepPace == 0)
        {
            apu.ch4.env_sweep_en = false;
            apu.ch4.volume = apu.pReg->NR42_Flags.InitialVolume;
        }
        else
        {
            apu.ch4.env_sweep_en = true;
            apu.ch4.env_dir = apu.pReg->NR42_Flags.EnvDir;
            apu.ch4.sweep_pace = apu.pReg->NR42_Flags.SweepPace;
        }
    }
}