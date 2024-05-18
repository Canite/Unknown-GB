#ifndef _GAME_H
#define _GAME_H

//#include <gb/gb.h>
#include <gbdk/platform.h>
#include <stdio.h>

#include "structs.h"
#include "input.h"
#include "physics.h"
#include "player.h"
#include "utils.h"
#include "gfx.h"
#include "levels.h"
#include "key_obj.h"
#include "sounds.h"
#include "../res/bs_redsandMachine_quiet.h"

// Game state
enum GameState
{
    GS_TITLE_LOAD,
    GS_TITLE,
    GS_END,
    GS_START_GAME,
    GS_INGAME,
    GS_PAUSE,
    GS_START_LEVEL,
    GS_FADE_OUT,
    GS_FADE_IN,
};

enum GameFlags
{
    GF_NONE = 0x00,
    GF_DOOR_OPEN = 0x01,
    GF_ITEM_PICKED = 0x02,
    GF_CHEAT_ENABLED = 0x80,
};

struct Game
{
    uint16_t gameState;
    uint16_t nextState;
    uint16_t gameFrame;
    uint16_t levelFrame;
    uint8_t currentLevel;
    uint16_t deaths;
    level_t level_data;
    uint16_t mapPixelW;
    uint16_t mapPixelH;
    uint8_t mapX;
    uint8_t mapY;
    uint8_t oldMapX;
    uint8_t oldMapY;
    uint8_t flags;
};

extern struct Game game;

void init_game(void);
void game_loop(void);
void start_level(void);

#endif
