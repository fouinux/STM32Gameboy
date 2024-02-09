#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "gameboy/cpu.h"
#include "gameboy/irq.h"
#include "gameboy/mem.h"
#include "gameboy/ppu.h"
#include "gameboy/timer.h"
#include "gameboy/apu.h"
#include "gameboy/joypad.h"
#include "gameboy/debug.h"

#define SCALE       2

uint8_t aBootROM[256];
uint8_t aGameROM[32 * 1024];

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

static inline int load_gamerom(const char *pFilename)
{
    FILE *pGameROM = fopen(pFilename, "rb");
    if (NULL == pGameROM)
    {
        printf("Cannot open GameROM file: %s\n", pFilename);
        return EXIT_FAILURE;
    }

    size_t size_read = fread(&aGameROM[0], sizeof(uint8_t), 32 * 1024, pGameROM);
    if (size_read != 32 * 1024)
    {
        printf("Error loading GameROM: %d\n", (int) size_read);
        return EXIT_FAILURE;
    }
    fclose(pGameROM);
    return EXIT_SUCCESS;
}

static inline void handle_keyboard(SDL_KeyboardEvent *pEvent)
{
    if (NULL == pEvent)
        return;

    bool state = (SDL_KEYDOWN == pEvent->type) ? true : false;

    switch (pEvent->keysym.sym)
    {
        case SDLK_UP:
            joypad_set_input(UP, state);
            break;
        case SDLK_DOWN:
            joypad_set_input(DOWN, state);
            break;
        case SDLK_RIGHT:
            joypad_set_input(RIGHT, state);
            break;
        case SDLK_LEFT:
            joypad_set_input(LEFT, state);
            break;
        case SDLK_KP_PERIOD:
            joypad_set_input(A, state);
            break;
        case SDLK_KP_0:
            joypad_set_input(B, state);
            break;
        case SDLK_DELETE:
            joypad_set_input(SELECT, state);
            break;
        case SDLK_END:
            joypad_set_input(START, state);
            break;
    }
}

int main(int argc, char *argv[])
{
    // Common vars
    SDL_Event event;
    bool cpuDebug = false;

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout, "SDL_Init failed(%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Load bootrom
    if (load_bootrom("DMG_ROM.bin"))
        return EXIT_FAILURE;

    // Load gamerom
    if (load_gamerom("Tetris.bin"))
    // if (load_gamerom("Super Mario Land.gb"))
        return EXIT_FAILURE;

    // Init gameboy emulator
    cpu_init();
    irq_init();
    mem_init();
    ppu_init();
    timer_init();
    apu_init();
    joypad_init();

    // Overwrite mem values
    mem_set_bootrom(&aBootROM[0]);
    mem_set_gamerom(&aGameROM[0], 0);
    mem_set_gamerom(&aGameROM[1024 * 16], 1);

    while(true)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
#ifdef DEBUG
                // Exec 10 more CPU cyles (help debug)
                for (int i = 0 ; i < 50 ; i++)
                    cpu_exec(true);
#endif
                break;
            }

            // Handle keyboard
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
                handle_keyboard(&event.key);
        }

        // if (cpu.reg.PC == 0x2F8)
        // {
        //     printf("GAME_STATUS\n");
        //     cpuDebug = true;
        // }

        // if (cpu.reg.PC == 0x100)
        //     cpuDebug = true;

        // if (cpu.reg.PC == 0x0293)
        // {
        //     mem_hexdump(0xFF80, 128);
        //     cpuDebug = true;
        // }

        // if (cpu.reg.PC == 0x02A0)
        // {
        //     mem_hexdump(0xFF80, 128);
        //     return 0;
        // }

        // Run Gameboy emulation
        cpu_exec(cpuDebug);
        ppu_exec();
        timer_exec();

    }

    // Clean SDL stuff
    ppu_destroy();

    // Clean up
    SDL_Quit();
    return EXIT_SUCCESS;
}
