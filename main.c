#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

uint8_t aBootROM[256];


static inline int load_bootrom(const char *pFilename)
{
    FILE *pBootROM = fopen(pFilename, "rb");
    if (NULL == pBootROM)
    {
        printf("Cannot open BootROM file: %s\n", pFilename);
        return EXIT_FAILURE;
    }

    size_t size_read = fread(&aBootROM[0], sizeof(uint8_t), 256, pBootROM);
    if (size_read != 256)
    {
        printf("Error loading BootROM: %d\n", (int) size_read);
        return EXIT_FAILURE;
    }
    fclose(pBootROM);
    return EXIT_SUCCESS;

}

static inline int load_gamerom(const char *pFilename, uint8_t **pGameROM, uint8_t *pBankROM)
{
    FILE *pFile = fopen(pFilename, "rb");
    if (NULL == pGameROM)
    {
        printf("Cannot open GameROM file: %s\n", pFilename);
        return EXIT_FAILURE;
    }

    // Get the size of the game
    fseek(pFile, 0L, SEEK_END);
    size_t game_size = ftell(pFile);
    rewind(pFile);

    // Allocate Game memory
    *pGameROM = (uint8_t*) malloc(game_size);
    if (NULL == *pGameROM)
    {
        printf("Cannot allocate memory for GameROM (%lu B)\n", game_size);
        return EXIT_FAILURE;
    }

    // Load Game
    size_t read_size = fread(*pGameROM, sizeof(uint8_t), game_size, pFile);
    if (read_size != game_size)
    {
        printf("Error loading GameROM: %d\n", (int) read_size);
        return EXIT_FAILURE;
    }
    fclose(pFile);

    // Return game size in ROM banks
    *pBankROM = game_size / MEM_CARTRIDGE_ROM_BANK_SIZE;

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    // Common vars
    bool cpuDebug = false;
    bool render = false;

    // Boot & Game ROMs
    uint8_t gameROMBanks = 0;
    uint8_t *pGameROM;

    // Load bootrom
    if (load_bootrom("DMG_ROM.bin"))
        return EXIT_FAILURE;

    // Load gamerom
    if (argc < 2)
    {
        printf("Missing game rom filename\n");
        return EXIT_FAILURE;
    }
    if (load_gamerom(argv[1], &pGameROM, &gameROMBanks))
        return EXIT_FAILURE;

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

    // Load Boot ROM and game ROM
    mem_set_bootrom(&aBootROM[0]);
    mem_load_gamerom(pGameROM);

    while(true)
    {
        // if (cpu.reg.PC == 0x100)
        //     cpuDebug = true;

        // Run Gameboy emulation
        cpu_exec(cpuDebug);
        render = ppu_exec();
        timer_exec();
        // serial_exec();
        if (!msdl_loop(render))
        {
#ifdef DEBUG
                // Exec 10 more CPU cyles (help debug)
                for (int i = 0 ; i < 50 ; i++)
                    cpu_exec(true);
#endif
            break;
        }

#ifdef PPU_DEBUG
        if (render)
            msdl_render_debug();
#endif
    }

    free(pGameROM);

    // Clean SDL stuff
    msdl_deinit();

    return EXIT_SUCCESS;
}
