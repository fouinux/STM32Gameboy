#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gameboy/cpu.h"
#include "gameboy/irq.h"
#include "gameboy/mem.h"
#include "gameboy/ppu.h"
#include "gameboy/timer.h"
#include "gameboy/apu.h"
#include "gameboy/joypad.h"
#include "gameboy/serial.h"
#include "gameboy/debug.h"

#include "msdl.h"

#define BOOTROM_FILENAME    "DMG_ROM.gb"

void skip_boot(void)
{
    cpu.reg.AF = 0x01B0;
    cpu.reg.BC = 0x0013;
    cpu.reg.DE = 0x00D8;
    cpu.reg.HL = 0x014D;
    cpu.reg.SP = 0xFFFE;
    cpu.reg.PC = 0x0100;

    // Disable bootrom
    mem_write_u8(0xFF50, 0x01);
}

int main(int argc, char *argv[])
{
    // Common vars
    bool render = false;

    // Load gamerom
    if (argc < 2)
    {
        printf("Missing game rom filename\n");
        return EXIT_FAILURE;
    }

    if (msdl_init())
        return EXIT_FAILURE;

    // Init gameboy emulator
    cpu_init();
    irq_init();
    mem_init();
    ppu_init();
    timer_init();
    apu_init();
    joypad_init();
    serial_init();

    debug_init();

    // Load Boot ROM and game ROM
    mem_load_bootrom(BOOTROM_FILENAME);
    mem_load_gamerom(argv[1]);

    // skip_boot();

    while(true)
    {
        // if (cpu.reg.PC == 0x100)
        //     cpu.debug = true;

        // Run Gameboy emulation
        cpu_exec();
        render = ppu_exec();
        timer_exec();
        serial_exec();
        if (!msdl_loop(render))
        {
#ifdef DEBUG
            // Exec 10 more CPU cyles (help debug)
            debug.cpu = true;
            for (int i = 0 ; i < 50 ; i++)
                cpu_exec();
#endif
            break;
        }

#ifdef PPU_DEBUG
        if (render)
            msdl_render_debug();
#endif
    }

    mem_deinit();

    // Clean SDL stuff
    msdl_deinit();

    return EXIT_SUCCESS;
}
