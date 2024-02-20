/*
 * msdl.h
 *
 *  Created on: 20 feb. 2024
 *      Author: Guillaume Fouilleul
 */

#include "msdl.h"
#include "ppu.h"
#include "joypad.h"

#include <SDL2/SDL.h>

struct msdl_window_t
{
    SDL_Window* pWindow;
    SDL_Texture* pTexture;
    SDL_Renderer *pRenderer;
};

struct msdl_t
{
    struct msdl_window_t main;
    uint32_t aColor[4];
};

struct msdl_t msdl;

int msdl_init(void)
{
    uint8_t *pPixels;
    int pitch;

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout, "SDL_Init failed(%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create Display Window
    msdl.main.pWindow = SDL_CreateWindow("STM32 Gameboy",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               PPU_SCREEN_W * MSDL_SCALE,
                               PPU_SCREEN_H * MSDL_SCALE,
                               SDL_WINDOW_OPENGL);
    if(NULL == msdl.main.pWindow)
    {
        // In the case that the window could not be made...
        printf("Could not create display window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    msdl.main.pRenderer = SDL_CreateRenderer(msdl.main.pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    msdl.main.pTexture = SDL_CreateTexture(msdl.main.pRenderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                PPU_SCREEN_W,
                                PPU_SCREEN_H);

    // Alloc colors
    SDL_PixelFormat *pPixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    msdl.aColor[0] = SDL_MapRGBA(pPixelFormat, 0xFF, 0xFF, 0xFF, 0xFF); // White
    msdl.aColor[1] = SDL_MapRGBA(pPixelFormat, 0xAA, 0xAA, 0xAA, 0xFF); // Light gray
    msdl.aColor[2] = SDL_MapRGBA(pPixelFormat, 0x55, 0x55, 0x55, 0xFF); // Dark gray
    msdl.aColor[3] = SDL_MapRGBA(pPixelFormat, 0x00, 0x00, 0x00, 0xFF); // Black
    ppu_set_colors(&msdl.aColor[0]);

    SDL_LockTexture(msdl.main.pTexture, NULL, (void**) &pPixels, &pitch);
    ppu_set_video_buffer(pPixels, pitch);

    return EXIT_SUCCESS;
}

void msdl_deinit(void)
{
    SDL_DestroyWindow(msdl.main.pWindow);
    SDL_Quit();
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

bool msdl_loop(bool Render)
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;

        // Handle keyboard
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            handle_keyboard(&event.key);
    }

    if (Render)
    {
        uint8_t *pPixels;
        int pitch;
        SDL_UnlockTexture(msdl.main.pTexture);
        SDL_RenderClear(msdl.main.pRenderer);
        SDL_RenderCopy(msdl.main.pRenderer, msdl.main.pTexture, NULL, NULL);
        SDL_RenderPresent(msdl.main.pRenderer);
        SDL_LockTexture(msdl.main.pTexture, NULL, (void**) &pPixels, &pitch);
        ppu_set_video_buffer(pPixels, pitch);
    }

    return true;
}
