#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "gameboy/cpu.h"
#include "gameboy/irq.h"
#include "gameboy/mem.h"
#include "gameboy/ppu.h"

uint8_t aBootROM[256];
uint8_t aGameROM[32 * 1024];

static int load_bootrom(const char *pFilename)
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

}static int load_gamerom(const char *pFilename)
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

int main(int argc, char *argv[])
{
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout, "SDL_Init failed(%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Create window */
    SDL_Window* pWindow = NULL;
    pWindow = SDL_CreateWindow("SDL Gameboy",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               160,
                               144,
                               SDL_WINDOW_OPENGL);
    if(NULL == pWindow)
    {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

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

    // Overwrite mem values
    mem_set_bootrom(&aBootROM[0]);
    mem_set_gamerom(&aGameROM[0], 0);
    mem_set_gamerom(&aGameROM[1024 * 16], 1);

//    while(true)
    for (int i = 0 ;  i < 100 ; i++)
    {
        cpu_exec();
        ppu_exec();
    }

    // Close and destroy the window
    SDL_DestroyWindow(pWindow);

    // Clean up
    SDL_Quit();
    return EXIT_SUCCESS;
}
