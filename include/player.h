#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdint.h>
//#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>

#include "utils.h"
#include "../res/hook.h"
#include "../res/slime.h"

struct Player
{
    uint16_t x;
    uint16_t y;
    uint16_t oldX;
    uint16_t oldY;
    uint8_t w;
    uint8_t h;
    int16_t xSpd;
    int16_t ySpd;
    uint8_t flags;
    uint8_t colFlags;
    uint8_t oldColFlags;
    int16_t angularAcc;
    int16_t angularVel;
    uint8_t facing;
    uint8_t lookState;
    uint8_t animIndex;
    uint8_t animFrame;
    uint8_t numAnimFrames;
    uint8_t animSpeed;
    uint8_t grounded;
    uint8_t soundDelay;
    uint8_t fallDelay;
    uint8_t iFrames;
    uint8_t hookState;
    uint16_t hookX;
    uint16_t hookY;
    uint8_t hookAngle;
    uint8_t hookLength;
    uint8_t oldHookLength;
    uint8_t hookSegments;
};

enum PlayerFlags
{
    PF_NONE = 0x00,
    PF_HASKEY = 0x01,
    PF_HASINPUT = 0x02,
    PF_HASATL = 0x04,
};

enum HookState
{
    HS_STOWED,
    HS_LAUNCHED,
    HS_ATTACHED
};

enum LookState
{
    LS_NONE,
    LS_DOWN,
    LS_UP
};

#define GROUND_IDLE_ANIM_INDEX  0
#define GROUND_IDLE_ANIM_FRAMES  1
#define GROUND_IDLE_ANIM_SPEED  1

#define AIR_IDLE_ANIM_INDEX  1
#define AIR_IDLE_ANIM_FRAMES  1
#define AIR_IDLE_ANIM_SPEED  1

#define GROUND_MOVE_ANIM_INDEX  2
#define GROUND_MOVE_ANIM_FRAMES  4
#define GROUND_MOVE_ANIM_SPEED  7

#define CROUCH_ANIM_INDEX 6
#define CROUCH_ANIM_FRAMES 1
#define CROUCH_ANIM_SPEED 1

#define LOOK_ANIM_INDEX 7
#define LOOK_ANIM_FRAMES 1
#define LOOK_ANIM_SPEED 1

#define PLAYER_SPRITE_INDEX 0
#define PLAYER_SPRITE_TILE_INDEX 0

#define HOOK_SPRITE_INDEX 1
#define HOOK_SPRITE_TILE_INDEX 8

#define HOOK_INDICATOR_SPRITE_INDEX 1
#define HOOK_INDICATOR_SPRITE_TILE_INDEX 9

extern struct Player player;

void init_player(uint8_t initSprites, uint16_t startX, uint16_t startY);
void handle_input(void);

#endif
