// This is adapted from https://github.com/untoxa/VGM2GBSFX/
#ifndef _SOUNDS_H
#define _SOUNDS_H

#include <gbdk/platform.h>
#include <stdint.h>

#include "hUGEDriver.h"

// SFX priority constants: concurrent effect will play only if its priority level is higher or equal
#define MUSIC_SFX_PRIORITY_MINIMAL  0
#define MUSIC_SFX_PRIORITY_NORMAL   4
#define MUSIC_SFX_PRIORITY_HIGH     8

// SFX channel constants
#define SFX_CH_1 1
#define SFX_CH_2 2
#define SFX_CH_3 4
#define SFX_CH_4 8

// masks applied to the ISR skip counter
#define MUSIC_TICK_MASK_60HZ 0x00u
#define MUSIC_TICK_MASK_256HZ 0x03u

#define MUSIC_STOP_BANK 0xffu

#define SFX_CH_RETRIGGER  0b11000000
#define SFX_CH_ENABLE     0b10000000

// muting masks
#define MUTE_MASK_NONE 0
#define MUTE_MASK_WAVE MUSIC_CH_3

// mute mask helper macro
#define SFX_MUTE_MASK(VARNAME) ( (uint8_t) & __mute_mask_ ## VARNAME )

extern volatile uint8_t music_current_track_bank;
extern volatile uint8_t music_mute_mask;
extern const hUGESong_t* music_next_track;
extern volatile uint8_t music_sfx_priority;
extern uint8_t music_tick_mask;
extern uint8_t start_music;

static uint8_t music_paused = FALSE;

extern volatile uint8_t sfx_play_bank;
extern const uint8_t * sfx_play_sample;
extern uint8_t sfx_frame_skip;

void music_play_isr(void) NONBANKED;
uint8_t sfx_play_isr(void) NONBANKED;
void music_pause(uint8_t pause) NONBANKED;

// set up timer interrupt to 256Hz and set up driver for 256Hz
inline void music_setup_timer_ex(uint8_t is_fast);

// set up timer interrupt to 256Hz and set up driver for 256Hz
inline void music_setup_timer(void);

// cut sound on all channels
inline void music_sound_cut(void);

// initialize the SFX driver
inline void sfx_sound_init(void);

// stop playing SFX
inline void sfx_reset_sample(void);

// cut sound on the selected channels
inline void sfx_sound_cut_mask(uint8_t mask);

// cut sound on the selected channels
inline void music_sound_cut_mask(uint8_t mask);

// start playing SFX
inline void sfx_set_sample(uint8_t bank, const uint8_t * sample);

// initialize the music and SFX driver
inline void music_init(void);

inline void music_load(uint8_t bank, const hUGESong_t* data);

// play SFX with the desired priority, muting the music on the selected channels
inline void music_play_sfx(uint8_t bank, const uint8_t * sample, uint8_t mute_mask, uint8_t priority);

#endif
