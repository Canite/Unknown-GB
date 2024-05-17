#include <stdint.h>
//#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <gbdk/incbin.h>

#include "../include/player.h"
#include "../include/game.h"
#include "../include/gfx.h"
#include "../include/input.h"
#include "asm/types.h"

#if defined(NINTENDO)
void VBL_isr(void)
{
    if (gfx.draw_window)
    {
        WX_REG = DEVICE_WINDOW_PX_OFFSET_X;
        SHOW_WIN;
    }
}

void LCD_isr(void)
{
    HIDE_WIN;
    WX_REG = 0;
}

#elif defined(NINTENDO_NES)
void VBL_isr(void)
{

}

void LCD_isr(void)
{

}

extern uint8_t _lcd_scanline;

#endif

int main(void)
{
    DISPLAY_OFF;
    SPRITES_8x8;

#if defined(NINTENDO)
    BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY);
    OBP1_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    // Enable sound
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x10;

    music_init();

    CRITICAL {
        STAT_REG |= STATF_LYC;
        music_setup_timer();

        add_VBL(VBL_isr);
        add_LCD(LCD_isr);
        add_low_priority_TIM(music_play_isr);
        LYC_REG = 8;
    }
    set_interrupts(LCD_IFLAG | VBL_IFLAG | IE_REG | TIM_IFLAG);

#elif defined(NINTENDO_NES)
    add_VBL(VBL_isr);
    set_sprite_palette_entry(0,2, RGB8(255, 255, 255));
#endif

    init_title();
    init_gfx();
    init_game();

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    game_loop();

    return 1;
}
