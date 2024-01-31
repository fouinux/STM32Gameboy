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
    // uint8_t *pPixels;
    // int pitch;
    struct timespec timeStart, timeStop;
    // Main window
    // SDL_Texture* pTexture;
    // SDL_Window* pWindow = NULL;
    // SDL_Renderer *pRenderer;

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout, "SDL_Init failed(%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // /* Create main window */
    // pWindow = SDL_CreateWindow("Background Tile Map",
    //                            SDL_WINDOWPOS_UNDEFINED,
    //                            SDL_WINDOWPOS_UNDEFINED,
    //                            PPU_BG_W * SCALE,
    //                            PPU_BG_H * SCALE,
    //                            SDL_WINDOW_OPENGL);
    // if(NULL == pWindow)
    // {
    //     // In the case that the window could not be made...
    //     printf("Could not create window: %s\n", SDL_GetError());
    //     return EXIT_FAILURE;
    // }
    // pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
    // pTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
    //                              PPU_BG_W, PPU_BG_H);

    // Load bootrom
    if (load_bootrom("DMG_ROM.bin"))
        return EXIT_FAILURE;

    // Load gamerom
    if (load_gamerom("Tetris.bin"))
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
#ifdef debug
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

        // Run Gameboy emulation
        // if (ppu.pReg->LCDC_Flags.DisplayEnable == 0)
        // {
            cpu_exec(false);
            ppu_exec();
            timer_exec();
        // }
        // else
        // {
        //     clock_gettime(CLOCK_MONOTONIC, &timeStart);
        //     cpu_exec(false);
        //     clock_gettime(CLOCK_MONOTONIC, &timeStop);
        //     debug_print_clock_diff("CPU", &timeStart, &timeStop);

        //     clock_gettime(CLOCK_MONOTONIC, &timeStart);
        //     ppu_exec();
        //     clock_gettime(CLOCK_MONOTONIC, &timeStop);
        //     debug_print_clock_diff("PPU", &timeStart, &timeStop);

            
        //     clock_gettime(CLOCK_MONOTONIC, &timeStart);
        //     timer_exec();
        //     clock_gettime(CLOCK_MONOTONIC, &timeStop);
        //     debug_print_clock_diff("TIMER", &timeStart, &timeStop);
        // }


        // if (cpu.reg.PC == 0x60)
        // {
        //     SDL_LockTexture(pTexture, NULL, (void**) &pPixels, &pitch);
        //     ppu_print_bg(pPixels, pitch);
        //     SDL_UnlockTexture(pTexture);

        //     SDL_RenderClear(pRenderer);
        //     SDL_RenderCopy(pRenderer, pTexture, NULL, NULL);
        //     SDL_RenderPresent(pRenderer);
        // }
    }

    ppu_destroy();

    // Close and destroy the window
    // SDL_DestroyWindow(pWindow);

    // Clean up
    SDL_Quit();
    return EXIT_SUCCESS;
}
