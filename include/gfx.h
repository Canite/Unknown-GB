#ifndef _GFX_H
#define _GFX_H

#include <gb/gb.h>
#include <stdlib.h>
#include <gbdk/platform.h>

#include "utils.h"
#include "player.h"
#include "game.h"
#include "../res/font_bold.h"

#define BLANK_TILE_INDEX 0
#define OPEN_DOOR_TILE1_INDEX 1
#define OPEN_DOOR_TILE2_INDEX 2
#define CLOSED_DOOR_TILE1_INDEX 28
#define CLOSED_DOOR_TILE2_INDEX 29

#define FONT_START_TILE_INDEX 46
#define FONT_COLON_TILE_INDEX 56
#define FONT_CLOCK_TILE_INDEX 57
#define FONT_SKULL_TILE_INDEX 58
#define FONT_LV_TILE_INDEX 60
#define FONT_PAUSE_TILE_INDEX 61

struct Camera
{
    uint16_t x;
    uint16_t y;
    uint16_t oldX;
    uint16_t oldY;
    uint16_t targetX;
    uint16_t targetY;
    int8_t xSpd;
    int8_t ySpd;
    uint8_t redraw;
};

struct WindowData
{
    uint16_t timer_frames;
    uint8_t timer_minutes;
    uint8_t timer_seconds;
    uint16_t drawn_deaths;
    uint8_t drawn_level;
    uint8_t level_timer_tiles[6];
    uint8_t death_counter_tiles[4];
    uint8_t level_counter_tiles[3];
};

struct GraphicsInfo
{
    uint8_t fade_delay;
    uint8_t fade_step_length;
    uint8_t sprites_inited;
    uint8_t draw_window;
    uint8_t update_background;
};


extern struct Camera camera;
extern struct WindowData window;
extern struct GraphicsInfo gfx;

void init_gfx(void);
void init_camera(void);
void init_window(void);
void update_window(void);
void update_camera(void);
void set_camera_target(void);
void update_camera_coordinates(void);
void clear_background(void);
void fade_out(void);
void fade_in(void);
void update_game_sprites(void);
void update_background(void);
void draw_end_screen(void);
void draw_hook(void);
inline void hide_atl(void);
inline void hide_key(void);
inline void hide_hook(void);
void draw_hook_indicator(void);

#endif
