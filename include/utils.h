#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
//#include <gb/gb.h>
#include <gbdk/platform.h>
#include "player.h"
#include "game.h"

// coordinate translation macros
#define SUBPIXELS_TO_PIXELS(v) (v >> 4)
#define PIXELS_TO_SUBPIXELS(v) (v << 4)

#define TOP_RIGHT_COL 0x1
#define TOP_LEFT_COL  0x2
#define BOT_RIGHT_COL 0x4
#define BOT_LEFT_COL  0x8

extern const int8_t sine_wave[256];

#define SIN(a)  (sine_wave[(uint8_t)(a)])
#define COS(a)  (sine_wave[(uint8_t)((uint8_t)(a) + 64u)])

uint16_t isqrt(uint16_t x);

inline int16_t abs16(int16_t x);
inline int8_t abs8(int8_t x);
inline uint16_t abs_sub(uint16_t x, uint16_t y);
inline int16_t sign(int16_t x);

#endif
