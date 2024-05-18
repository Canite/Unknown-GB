#ifndef _STRUCTS_H
#define _STRUCTS_H

//#include <gb/gb.h>
#include <gbdk/platform.h>
#include <stdio.h>

typedef struct level_t 
{
    const unsigned char * tiles;
    uint8_t tiles_bank;
    const unsigned char * collisions;
    uint8_t collisions_bank;
    uint16_t tile_width;
    uint16_t tile_height;
    uint16_t spawn_x;
    uint16_t spawn_y;
    uint8_t door_open;
} level_t;

typedef struct basic_obj_t
{
    uint16_t x;
    uint16_t y;
    uint8_t w;
    uint8_t h;
    int16_t xSpd;
    int16_t ySpd;
} basic_obj_t;

#endif
