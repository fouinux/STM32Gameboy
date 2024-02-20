/*
 * apu.h
 *
 *  Created on: 23 janv. 2024
 *      Author: Guillaume Fouilleul
 */

#ifndef INC_GAMEBOY_APU_H_
#define INC_GAMEBOY_APU_H_

#include <stdint.h>

struct apu_reg_t
{
    union
    {
        uint8_t NR10; // 0xFF10 : Channel 1 sweep
        struct
        {
            uint8_t IndividualStep : 3;
            uint8_t Direction: 1;
            uint8_t Pace: 3;
            uint8_t : 1;
        } NR10_Flags;
    };

    union
    {
        uint8_t NR11; // 0xFF11 : Channel 1 length timer & duty cycle
        struct
        {
            uint8_t Length : 6;
            uint8_t DutyCycle : 2;
        } NR11_Flags;
    };

    union
    {
        uint8_t NR12; // 0xFF12 : Channel 1 volume & envelope
        struct
        {
            uint8_t SweepPace : 3;
            uint8_t EnvDir : 1;
            uint8_t InitialVolume : 4;
        } NR12_Flags;
    };

    uint8_t NR13; // 0xFF13 : Channel 1 period low (W)

    union
    {
        uint8_t NR14; // 0xFF14 : Channel 1 period high & control
        struct
        {
            uint8_t Period : 3;
            uint8_t : 3;
            uint8_t LengthEnable : 1;
            uint8_t Trigger : 1;
        } NR14_Flags;
    };

    uint8_t : 8; // 0xFF15 : Padding

    union
    {
        uint8_t NR21; // 0xFF16 : Channel 2 length timer & duty cycle
        struct
        {
            uint8_t Length : 6;
            uint8_t DutyCycle : 2;
        } NR21_Flags;
    };

    union
    {
        uint8_t NR22; // 0xFF17 : Channel 2 volume & envelope
        struct
        {
            uint8_t SweepPace : 3;
            uint8_t EnvDir : 1;
            uint8_t InitialVolume : 4;
        } NR22_Flags;
    };

    uint8_t NR23; // 0xFF18 : Channel 2 period low (W)

    union
    {
        uint8_t NR24; // 0xFF19 : Channel 2 period high & control
        struct
        {
            uint8_t Period : 3;
            uint8_t : 3;
            uint8_t LengthEnable : 1;
            uint8_t Trigger : 1;
        } NR24_Flags;
    };

    union
    {
        uint8_t NR30; // 0xFF1A : Channel 3 DAC enable
        struct
        {
            uint8_t : 7;
            uint8_t DAC_Enable : 1;
        } NR30_Flags;
    };

    uint8_t NR31; // 0xFF1B : Channel 3 length timer (W)

    union
    {
        uint8_t NR32; // 0xFF1C : Channel 3 output level
        struct
        {
            uint8_t : 5;
            uint8_t OutputLevel : 2;
            uint8_t : 1;
        } NR32_Flags;
    };

    uint8_t NR33; // 0xFF1D : Channel 3 period low (W)

    union
    {
        uint8_t NR34; // 0xFF1E : Channel 3 period high & control
        struct
        {
            uint8_t Period : 3;
            uint8_t : 3;
            uint8_t LengthEnable : 1;
            uint8_t Trigger : 1;
        } NR34_Flags;
    };

    uint8_t : 8; // 0xFF1F : Padding

    union
    {
        uint8_t NR41; // 0xFF20 : Channel 4 length time (W)
        struct
        {
            uint8_t Length : 6;
            uint8_t : 2;
        } NR41_Flags;
    };

    union
    {
        uint8_t NR42; // 0xFF21 : Channel 4 volume & envelope
        struct
        {
            uint8_t SweepPace : 3;
            uint8_t EnvDir : 1;
            uint8_t InitialVolume : 4;
        } NR42_Flags;
    };

    union
    {
        uint8_t NR43; // 0xFF22 : Channel 4 frequency & randomness
        struct
        {
            uint8_t ClockDivicer : 3;
            uint8_t LFSRWidth : 1;
            uint8_t ClockShift : 4;
        } NR43_Flags;
    };

    union
    {
        uint8_t NR44; // 0xFF23 : Channel 4 control
        struct
        {
            uint8_t : 6;
            uint8_t LengthEnable : 1;
            uint8_t Trigger : 1;
        } NR44_Flags;
    };

    union
    {
        uint8_t NR50; // 0xFF24 : Master volume & VIN panning
        struct
        {
            uint8_t RightVolume: 3;
            uint8_t VIN_Right : 1;
            uint8_t LeftVolume: 3;
            uint8_t VIN_Left : 1;
        } NR50_Flags;
    };

    union
    {
        uint8_t NR51; // 0xFF25 : Sound panning
        struct
        {
            uint8_t CH1_Right: 1;
            uint8_t CH2_Right: 1;
            uint8_t CH3_Right: 1;
            uint8_t CH4_Right: 1;
            uint8_t CH1_Left : 1;
            uint8_t CH2_Left : 1;
            uint8_t CH3_Left : 1;
            uint8_t CH4_Left : 1;
        } NR51_Flags;
    };

    union
    {
        uint8_t NR52; // 0xFF24 : Audio master control
        struct
        {
            uint8_t CH1_Enable: 1;
            uint8_t CH2_Enable: 1;
            uint8_t CH3_Enable: 1;
            uint8_t CH4_Enable: 1;
            uint8_t : 3;
            uint8_t AudioEnable : 1;
        } NR52_Flags;
    };

    uint8_t : 8; // 0xFF25 : Padding
    uint8_t : 8; // 0xFF26 : Padding
    uint8_t : 8; // 0xFF27 : Padding
    uint8_t : 8; // 0xFF28 : Padding
    uint8_t : 8; // 0xFF29 : Padding
    uint8_t : 8; // 0xFF2A : Padding
    uint8_t : 8; // 0xFF2B : Padding
    uint8_t : 8; // 0xFF2C : Padding
    uint8_t : 8; // 0xFF2D : Padding
    uint8_t : 8; // 0xFF2E : Padding
    uint8_t : 8; // 0xFF2F : Padding

    struct
    {
        uint8_t Lower: 4;
        uint8_t Upper: 4;
    } Wave[16]; // 0xFF30 - 0xFF3F : Wave pattern RAM
} __attribute__ ((__packed__));

struct apu_t
{
    struct apu_reg_t *pReg;

    // Channel 1 : Pulse
};

void apu_init(void);

#endif /* INC_GAMEBOY_APU_H_ */
