#include "../include/sounds.h"
#include "asm/types.h"

volatile uint8_t music_current_track_bank = MUSIC_STOP_BANK;
volatile uint8_t music_mute_mask = 0;
const hUGESong_t* music_next_track;
uint8_t music_play_isr_counter = 0;
uint8_t music_play_isr_pause = FALSE;
volatile uint8_t music_sfx_priority = MUSIC_SFX_PRIORITY_MINIMAL;
uint8_t music_tick_mask = MUSIC_TICK_MASK_256HZ;
uint8_t start_music = 0;

volatile uint8_t sfx_play_bank = MUSIC_STOP_BANK;
const uint8_t * sfx_play_sample = NULL;
uint8_t sfx_frame_skip;

uint8_t sfx_play_isr(void) NONBANKED NAKED
{
    return 0;
}

void music_play_isr(void) NONBANKED
{

}

void music_pause(uint8_t pause)
{

}

inline void music_setup_timer_ex(uint8_t is_fast)
{

}

// set up timer interrupt to 256Hz and set up driver for 256Hz
inline void music_setup_timer(void)
{

}

// cut sound on all channels
inline void music_sound_cut(void)
{

}

// initialize the SFX driver
inline void sfx_sound_init(void)
{

}

// stop playing SFX
inline void sfx_reset_sample(void)
{

}

// cut sound on the selected channels
inline void sfx_sound_cut_mask(uint8_t mask)
{

}

// cut sound on the selected channels
inline void music_sound_cut_mask(uint8_t mask)
{

}

// start playing SFX
inline void sfx_set_sample(uint8_t bank, const uint8_t * sample)
{

}

// initialize the music and SFX driver
inline void music_init(void)
{

}

inline void music_load(uint8_t bank, const hUGESong_t* data)
{

}

// play SFX with the desired priority, muting the music on the selected channels
inline void music_play_sfx(uint8_t bank, const uint8_t * sample, uint8_t mute_mask, uint8_t priority)
{

}
