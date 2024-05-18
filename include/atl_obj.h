#ifndef _ATL_H
#define _ATL_H

#include <stdint.h>
//#include <gb/gb.h>
#include <gbdk/platform.h>
#include "../res/atl.h"
#include "player.h"

struct Atl
{
    uint16_t x;
    uint16_t y;
    uint8_t w;
    uint8_t h;
    int16_t xSpd;
    int16_t ySpd;
};

extern struct Atl atl;

#define ATL_BACKGROUND_TILE_INDEX 39
#define ATL_SPRITE_TILE_INDEX 11
#define ATL_SPRITE_INDEX 12

#define ATL_ACCELERATION 3

void init_atl(uint8_t initSprites);

#endif
