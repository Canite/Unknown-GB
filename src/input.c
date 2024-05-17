#include "../include/input.h"

#pragma bank 255

BANKREF(joy)
uint8_t joy = 0;

BANKREF(old_joy)
uint8_t old_joy = 0;

BANKREF(process_title_input)
void process_title_input(void) BANKED
{
    if (INPUT_KEYPRESS(J_START))
    {
        music_play_sfx(BANK(sfx_11), sfx_11, SFX_MUTE_MASK(sfx_11), MUSIC_SFX_PRIORITY_NORMAL);
        game.gameState = GS_FADE_OUT;
        game.nextState = GS_START_GAME;
        game.currentLevel += 1;
        gfx.fade_delay = 30;
        gfx.fade_step_length = 5;
        if (INPUT_KEY(J_UP) && INPUT_KEY(J_B))
        {
            game.flags |= GF_CHEAT_ENABLED;
        }
    }
}

BANKREF(process_game_input)
void process_game_input(void) BANKED
{
    // DEBUG
    // level change
    if ((game.flags & GF_CHEAT_ENABLED) && INPUT_KEY(J_SELECT))
    {
        if (INPUT_KEYPRESS(J_LEFT))
        {
            game.currentLevel -= 1;
            start_level();
            return;
        }
        else if (INPUT_KEYPRESS(J_RIGHT))
        {
            game.currentLevel += 1;
            start_level();
            return;
        }
    }

    if (INPUT_KEYPRESS(J_START))
    {
        music_play_sfx(BANK(sfx_11), sfx_11, SFX_MUTE_MASK(sfx_11), MUSIC_SFX_PRIORITY_NORMAL);
        game.gameState = GS_PAUSE;
#if defined(NINTENDO)
        LYC_REG = 16;
#endif
        return;
    }

    player.flags &= ~PF_HASINPUT;
    if (INPUT_KEY(J_LEFT))
    {
        player.facing = 1;
        player.flags |= PF_HASINPUT;
        if (player.grounded)
        {
            if (player.animIndex != GROUND_MOVE_ANIM_INDEX)
            {
                player.animIndex = GROUND_MOVE_ANIM_INDEX;
                player.numAnimFrames = GROUND_MOVE_ANIM_FRAMES;
                player.animSpeed = GROUND_MOVE_ANIM_SPEED;
                player.animFrame = 0;
            }

            if (player.xSpd > -MAX_X_GROUND_SPEED_IN_SUBPIXELS) 
            {
                player.xSpd -= X_GROUND_ACCELERATION_IN_SUBPIXELS;
            }
        } 
        else if (player.hookState == HS_ATTACHED)
        {
            if (player.angularVel > (MIN_ANGULAR_VELOCITY + (player.hookLength >> 2)) &&
               (player.hookLength >= MIN_HOOK_LENGTH))
            {
                player.angularAcc += INPUT_ANGULAR_ACC / (player.hookLength >> 3);
            }
        }
        else
        {
            if (player.xSpd > -MAX_X_SPEED_IN_SUBPIXELS)
            {
                player.xSpd -= X_ACCELERATION_IN_SUBPIXELS;
            }
        }
    }
    else if (INPUT_KEY(J_RIGHT))
    {
        player.facing = 0;
        player.flags |= PF_HASINPUT;
        if (player.grounded)
        {
            if (player.animIndex != GROUND_MOVE_ANIM_INDEX)
            {
                player.animIndex = GROUND_MOVE_ANIM_INDEX;
                player.numAnimFrames = GROUND_MOVE_ANIM_FRAMES;
                player.animSpeed = GROUND_MOVE_ANIM_SPEED;
                player.animFrame = 0;
            }

            if (player.xSpd < MAX_X_GROUND_SPEED_IN_SUBPIXELS)
            {
                player.xSpd += X_GROUND_ACCELERATION_IN_SUBPIXELS;
            }
        } 
        else if (player.hookState == HS_ATTACHED)
        {
            if (player.angularVel < ((MAX_ANGULAR_VELOCITY) - (player.hookLength >> 2)) &&
               (player.hookLength >= MIN_HOOK_LENGTH))
            {
                player.angularAcc -= INPUT_ANGULAR_ACC / (player.hookLength >> 3);
            }
        }
        else
        {
            if (player.xSpd < MAX_X_SPEED_IN_SUBPIXELS)
            {
                player.xSpd += X_ACCELERATION_IN_SUBPIXELS;
            }
        }
    }
    else
    {
        // Idle
        if (player.grounded)
        {
            if (player.animIndex != GROUND_IDLE_ANIM_INDEX)
            {
                player.animIndex = GROUND_IDLE_ANIM_INDEX;
                player.numAnimFrames = GROUND_IDLE_ANIM_FRAMES;
                player.animSpeed = GROUND_IDLE_ANIM_SPEED;
                player.animFrame = 0;
            }
        }
    }

    if (INPUT_KEYPRESS(J_A))
    {
        if (player.grounded || player.fallDelay > 0)
        {
            player.ySpd = -JUMP_ACCELERATION_IN_SUBPIXELS;
            player.fallDelay = 0;
            if (player.animIndex != AIR_IDLE_ANIM_INDEX)
            {
                player.animIndex = AIR_IDLE_ANIM_INDEX;
                player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
                player.animSpeed = AIR_IDLE_ANIM_SPEED;
                player.animFrame = 0;
            }

            music_play_sfx(BANK(sfx_02), sfx_02, SFX_MUTE_MASK(sfx_02), MUSIC_SFX_PRIORITY_NORMAL);
        }
    }
    else if (player.ySpd < 0 && !player.grounded && INPUT_KEYRELEASE(J_A))
    {
        player.ySpd -= player.ySpd >> 1;
    }

    if (INPUT_KEYPRESS(J_UP))
    {
        if (tile_botleft == OPEN_DOOR_TILE1_INDEX || tile_botright == OPEN_DOOR_TILE1_INDEX || tile_topleft == OPEN_DOOR_TILE1_INDEX || tile_topright == OPEN_DOOR_TILE1_INDEX)
        {
            game.gameState = GS_FADE_OUT;
            game.nextState = GS_START_LEVEL;
            game.currentLevel += 1;
            gfx.fade_delay = 30;
            gfx.fade_step_length = 5;
            return;
        }
        else if (tile_botleft == CLOSED_DOOR_TILE1_INDEX || tile_botright == CLOSED_DOOR_TILE1_INDEX || tile_topleft == CLOSED_DOOR_TILE1_INDEX || tile_topright == CLOSED_DOOR_TILE1_INDEX)
        {
            if ((game.flags & GF_DOOR_OPEN) != 0)
            {
                game.gameState = GS_FADE_OUT;
                game.nextState = GS_START_LEVEL;
                game.currentLevel += 1;
                gfx.fade_delay = 30;
                gfx.fade_step_length = 5;
                return;
            }
        }
    }

    if (INPUT_KEY(J_UP))
    {
        if (player.hookState == HS_ATTACHED && player.hookLength > MIN_HOOK_LENGTH && (player.hookAngle > (255 - MAX_ADJUST_ANGLE) || player.hookAngle < MAX_ADJUST_ANGLE))
        {
            player.oldHookLength = player.hookLength;
            player.hookLength -= 1;
            uint8_t old_segments = player.hookSegments;
            player.hookSegments = player.hookLength >> 3;
            if (old_segments != player.hookSegments)
            {
                hide_sprite(HOOK_SPRITE_INDEX + player.hookSegments);
            }
        }
        else if (player.grounded && player.xSpd == 0)
        {
            if (player.animIndex != LOOK_ANIM_INDEX)
            {
                player.animIndex = LOOK_ANIM_INDEX;
                player.numAnimFrames = LOOK_ANIM_FRAMES;
                player.animSpeed = LOOK_ANIM_SPEED;
                player.animFrame = 0;
                player.lookState = LS_UP;
            }
        }
    }
    else if (INPUT_KEY(J_DOWN))
    {
        if (player.hookState == HS_ATTACHED && player.hookLength < MAX_HOOK_LENGTH && (player.hookAngle > (255 - MAX_ADJUST_ANGLE) || player.hookAngle < MAX_ADJUST_ANGLE))
        {
            player.oldHookLength = player.hookLength;
            player.hookLength += 1;
            player.hookSegments = player.hookLength >> 3;
        }
        else if (player.grounded && player.xSpd == 0)
        {
            if (player.animIndex != CROUCH_ANIM_INDEX)
            {
                player.animIndex = CROUCH_ANIM_INDEX;
                player.numAnimFrames = CROUCH_ANIM_FRAMES;
                player.animSpeed = CROUCH_ANIM_SPEED;
                player.animFrame = 0;
                player.lookState = LS_DOWN;
            }
        }
    }
    else
    {
        player.lookState = LS_NONE;
    }

    if (INPUT_KEYRELEASE(J_UP))
    {
        if (player.animIndex == LOOK_ANIM_INDEX)
        {
            player.animIndex = GROUND_IDLE_ANIM_INDEX;
            player.numAnimFrames = GROUND_IDLE_ANIM_FRAMES;
            player.animSpeed = GROUND_IDLE_ANIM_SPEED;
            player.animFrame = 0;
            player.lookState = LS_NONE;
        }
    }

    if (INPUT_KEYRELEASE(J_DOWN))
    {
        if (player.animIndex == CROUCH_ANIM_INDEX)
        {
            player.animIndex = GROUND_IDLE_ANIM_INDEX;
            player.numAnimFrames = GROUND_IDLE_ANIM_FRAMES;
            player.animSpeed = GROUND_IDLE_ANIM_SPEED;
            player.animFrame = 0;
            player.lookState = LS_NONE;
        }
    }


    if (INPUT_KEYPRESS(J_B))
    {
        player.fallDelay = 0;
        if (player.hookState != HS_ATTACHED)
        {
            int16_t xCheck = PIXELS_TO_SUBPIXELS(4);
            int16_t yCheck = -PIXELS_TO_SUBPIXELS(4);
            uint16_t maxDist = MAX_HOOK_DISTANCE;
            player.hookAngle = ANGLE_315DEG;

            if (player.lookState == LS_UP)
            {
                xCheck = 0;
                yCheck = -PIXELS_TO_SUBPIXELS(8);
                maxDist = MAX_STRAIGHT_HOOK_DISTANCE;
                player.hookAngle = ANGLE_0DEG;
            }
            else if (player.facing)
            {
                xCheck = -PIXELS_TO_SUBPIXELS(4);
                player.hookAngle = ANGLE_45DEG;
            }

            uint8_t col_flags = 0;
            int16_t xTmp = 0;//PIXELS_TO_SUBPIXELS(4);
            int16_t yTmp = 0;
            while (!col_flags && ((abs16(xTmp) + abs16(yTmp)) < maxDist))
            {
                xTmp += xCheck;
                yTmp += yCheck;
                col_flags = check_collision(player.x + xTmp, player.y + yTmp);
            }

            if ((abs16(xTmp) + abs16(yTmp)) >= maxDist || (abs16(xTmp) + abs16(yTmp)) <= MIN_HOOK_DISTANCE)
            {
                player.hookState = HS_LAUNCHED;
                player.hookLength = 64;
                player.oldHookLength = player.hookLength;
                player.hookSegments = 8;
            }
            else
            {
                player.hookX = player.x + xTmp;
                player.hookY = player.y + yTmp;
                uint16_t xSqr = (SUBPIXELS_TO_PIXELS(abs_sub(player.x, player.hookX)) * SUBPIXELS_TO_PIXELS(abs_sub(player.x, player.hookX)));
                uint16_t ySqr = (SUBPIXELS_TO_PIXELS(abs_sub(player.y, player.hookY)) * SUBPIXELS_TO_PIXELS(abs_sub(player.y, player.hookY)));
                player.hookState = HS_ATTACHED;
                player.hookLength = isqrt(xSqr + ySqr);
                player.oldHookLength = player.hookLength;
                player.hookSegments = player.hookLength >> 3;
                int16_t xMomentum = (player.xSpd);
                int16_t yMomentum = 0;
                if (player.ySpd >= 0)
                {
                    yMomentum = sign(xMomentum) * (player.ySpd >> 1);
                }
                else
                {
                    yMomentum = -sign(xMomentum) * ((-1 * player.ySpd) >> 1);
                }

                player.angularVel = xMomentum + yMomentum;
                player.xSpd = 0;
                player.ySpd = 0;
            }

            if (player.animIndex != AIR_IDLE_ANIM_INDEX)
            {
                player.animIndex = AIR_IDLE_ANIM_INDEX;
                player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
                player.animSpeed = AIR_IDLE_ANIM_SPEED;
                player.animFrame = 0;
            }

            player.grounded = 0;
        }
    }
    else if (INPUT_KEYRELEASE(J_B))
    {
        if (player.hookState == HS_ATTACHED)
        {
            player.hookState = HS_STOWED;
            if (player.hookLength >= MIN_HOOK_LENGTH)
            {
                //int16_t xMomentum = CLAMP((player.angularVel * COS(player.hookAngle)) >> 7, -MAX_X_SPEED_IN_SUBPIXELS, MAX_X_SPEED_IN_SUBPIXELS);
                int16_t xMomentum = ((player.angularVel >> 1) * COS(player.hookAngle)) >> 7;
                if (xMomentum > 0)
                {
                    xMomentum += (player.hookLength >> 2);
                }
                else if (xMomentum < 0)
                {
                    xMomentum -= (player.hookLength >> 2);
                }
                player.xSpd = xMomentum;

                //int16_t yMomentum = CLAMP((player.angularVel * SIN(player.hookAngle)) >> 7, -MAX_Y_SPEED_IN_SUBPIXELS, MAX_Y_SPEED_IN_SUBPIXELS);
                int16_t yMomentum = (player.angularVel * SIN(player.hookAngle)) >> 7;
                /*
                if (yMomentum > 0)
                {
                    yMomentum += (player.hookLength >> 3);
                }
                else if (yMomentum < 0)
                {
                    yMomentum -= (player.hookLength >> 3);
                }
                */
                player.ySpd = -yMomentum;
            }
            else
            {
                player.xSpd = 0;
                player.ySpd = 0;
            }
        }
        else if (player.hookState == HS_LAUNCHED)
        {
            player.hookState = HS_STOWED;
        }
    }
}

BANKREF(process_pause_input)
void process_pause_input(void) BANKED
{
    if (INPUT_KEYPRESS(J_START))
    {
        game.gameState = GS_INGAME;
#if defined(NINTENDO)
        LYC_REG = 8;
#endif
    }
    else
    {
    }
}
