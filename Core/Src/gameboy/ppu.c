/*
 * ppu.c
 *
 *  Created on: 20 juil. 2020
 *      Author: Guillaume Fouilleul
 */

#include <gameboy/ppu.h>
#include <gameboy/mem.h>

#define STATE_HBLANK_DURATION       51
#define STATE_VBLANK_DURATION       114
#define STATE_OAM_SEARCH_DURATION   20
#define STATE_PXL_XFER_DURATION     43

#define LINE_VISIBLE_MAX            144
#define LINE_MAX                    154

#define OAM_NB                      40

struct oam_entry_t
{
    uint8_t Y;
    uint8_t X;
    uint8_t Number;
    union
    {
        uint8_t Flags;
        struct
        {
            uint8_t Priority : 1;
            uint8_t Y_Flip : 1;
            uint8_t X_Flip : 1;
            uint8_t Palette : 1;
            uint8_t : 4;
        };
    };
};

struct ppu_t ppu;

/**
 * Search of 10 visible sprites
 */
static inline void exec_oam_search(void)
{
    struct oam_entry_t *pOam = (struct oam_entry_t *) mem_get_oam_ram();
    uint8_t sprite_size = ppu.pReg->LCDC_Flags.OBJSize ? 16 : 8;

    // Reset OAM counter
    ppu.OAM_counter = 0;

    for (int i = 0 ;  i < OAM_NB ; i++)
    {
        // Search for Sprite on the line
        if (pOam[i].X != 0)
        {
            if (ppu.y + 0x10 >= pOam[i].Y)
            {
                if (ppu.y + 0x10 < pOam[i].Y + sprite_size)
                {
                    // Sprite is visible
                    ppu.aOAM_visible[ppu.OAM_counter++] = i;
                    if (ppu.OAM_counter >= PPU_OAM_VISIBLE_MAX)
                    {
                        // Stop search
                        break;
                    }
                }
            }
        }
    }

    // TODO Sort sprite array?
}

void ppu_init(void)
{
    ppu.pReg = (struct ppu_reg_t *) mem_get_register(PPU);

    ppu.state = STATE_OAM_SEARCH;
    ppu.state_counter = 0;

    // Start at y = 0 & x = 0
    ppu.y = 0;
    ppu.x = 0;
}

void ppu_exec(void)
{
    switch(ppu.state)
    {
        case STATE_HBLANK:
            if (ppu.state_counter >= STATE_HBLANK_DURATION)
            {
                ppu.y++;
                ppu.state_counter = 0;
                if (ppu.y >= LINE_VISIBLE_MAX)
                    ppu.state = STATE_VBLANK;
                else
                    ppu.state = STATE_OAM_SEARCH;
            }
            break;

        case STATE_VBLANK:
            if (ppu.state_counter >= STATE_VBLANK_DURATION)
            {
                ppu.y++;
                ppu.state_counter = 0;
                if (ppu.y >= LINE_MAX)
                    ppu.state = STATE_OAM_SEARCH;
                else
                    ppu.state = STATE_VBLANK_DURATION;
            }
            break;

        case STATE_OAM_SEARCH:
            if (ppu.state_counter == 0)
                exec_oam_search();
            if (ppu.state_counter >= STATE_OAM_SEARCH_DURATION)
            {
                ppu.state_counter = 0;
                ppu.state = STATE_PXL_XFER;
            }
            break;

        case STATE_PXL_XFER:
            if (ppu.state_counter >= STATE_PXL_XFER_DURATION)
            {
                ppu.state_counter = 0;
                ppu.state = STATE_HBLANK;
            }
            break;
    }
}
