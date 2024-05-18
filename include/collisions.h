#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <stdint.h>
//#include <gb/gb.h>
#include <gbdk/platform.h>
#include "player.h"
#include "game.h"
#include "levels.h"

extern uint8_t tile_botleft;
extern uint8_t tile_botright;
extern uint8_t tile_topleft;
extern uint8_t tile_topright;

extern uint8_t collision_botleft;
extern uint8_t collision_botright;
extern uint8_t collision_topleft;
extern uint8_t collision_topright;

extern uint8_t collision_size_offset;

enum CollisionType
{
    COL_EMPTY,
    COL_GOAL,
    COL_DEATH,
    COL_SOLID,
    COL_BOOST_RIGHT,
    COL_BOOST_UP,
};

uint8_t check_collision(uint16_t x, uint16_t y);
void update_tilemap_collision(uint16_t x, uint16_t y);
uint8_t check_rect_collision(uint16_t x1, uint16_t y1, uint8_t w1, uint8_t h1, uint16_t x2, uint16_t y2, uint8_t w2, uint8_t h2);
uint8_t handle_collision_h(int16_t xTmp, uint8_t col_flags);
uint8_t handle_collision_v(int16_t yTmp, uint8_t col_flags);
uint8_t handle_collision_v_corners(int16_t yTmp, uint8_t col_flags);

#endif
