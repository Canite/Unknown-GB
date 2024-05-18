#ifndef _KEY_H
#define _KEY_H

#include <stdint.h>
//#include <gb/gb.h>
#include <gbdk/platform.h>
#include "../res/key.h"
#include "player.h"

struct Key
{
    uint16_t x;
    uint16_t y;
    uint8_t w;
    uint8_t h;
    int16_t xSpd;
    int16_t ySpd;
};

extern struct Key key;

#define KEY_BACKGROUND_TILE_INDEX 38
#define KEY_SPRITE_TILE_INDEX 10
#define KEY_SPRITE_INDEX 11

#define KEY_ACCELERATION 3

void init_key(uint8_t initSprites);

#endif
