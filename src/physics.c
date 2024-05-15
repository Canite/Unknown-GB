#include "../include/physics.h"

#pragma bank 255

static void move_obj_to_player(basic_obj_t* obj)
{
    uint16_t objTargetX = player.x;
    uint16_t objTargetY = player.y - 16;

    if (player.facing)
    {
        objTargetX += 112;
    }
    else 
    {
        objTargetX -= 112;
    }

    if (obj->x != objTargetX || obj->y != objTargetY)
    {
        if (obj->x > objTargetX)
        {
            obj->xSpd = -((obj->x - objTargetX) >> 3);
        }
        else 
        {
            obj->xSpd = (objTargetX - obj->x) >> 3;
        }

        if (obj->y > objTargetY)
        {
            obj->ySpd = -((obj->y - objTargetY) >> 3);
        }
        else 
        {
            obj->ySpd = (objTargetY - obj->y) >> 3;
        }

        obj->x += obj->xSpd;
        obj->y += obj->ySpd;
    }
}

BANKREF(apply_physics)
void apply_physics(void) BANKED
{
    if (player.fallDelay)
    {
        player.fallDelay -= 1;
    }
    if (player.iFrames)
    {
        player.iFrames -= 1;
    }
    if (player.soundDelay)
    {
        player.soundDelay -= 1;
    }

    // Unhooked physics
    if (player.hookState != HS_ATTACHED)
    {
        player.oldColFlags = player.colFlags;
        int8_t xSpdSign = sign(player.xSpd);
        int8_t ySpdSign = sign(player.ySpd);
        int16_t xTmp = 0;
        int16_t yTmp = 0;
        uint8_t prevGrounded = player.grounded;
        player.grounded = 0;
        while (xTmp != player.xSpd || yTmp != player.ySpd)
        {
            if ((xSpdSign < 0 && (xTmp - 112) < player.xSpd) || (xSpdSign >= 0 && (xTmp + 112) > player.xSpd))
            {
                xTmp = player.xSpd;
            }
            else
            {
                xTmp += (((player.xSpd) < (0)) ? (-112) : (112));
            }

            if ((ySpdSign < 0 && (yTmp - 112) < player.ySpd) || (ySpdSign >= 0 && (yTmp + 112) > player.ySpd))
            {
                yTmp = player.ySpd;
            }
            else
            {
                yTmp += (((player.ySpd) < (0)) ? (-112) : (112));
            }

            // Check at least 1 pixel offset
            uint16_t yColOffset = yTmp;
            if (yTmp < 0) yColOffset = MIN(-16, yTmp);
            if (yTmp > 0) yColOffset = MAX(16, yTmp);
            uint16_t xColOffset = xTmp;
            if (xTmp < 0) xColOffset = MIN(-16, xTmp);
            if (xTmp > 0) xColOffset = MAX(16, xTmp);

            uint8_t col_flags = check_collision(player.x + xColOffset, player.y + yColOffset);
            player.colFlags = col_flags;
            // Check corners first
            uint8_t cornerCollision = handle_collision_v_corners(yColOffset, col_flags);
            if (cornerCollision == 2)
            {
                yTmp = 0;
                col_flags = check_collision(player.x + xColOffset, player.y);
                if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
            }
            else if (cornerCollision == 1)
            {
                if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
                col_flags = check_collision(player.x, player.y + yColOffset);
                if (handle_collision_v(yColOffset, col_flags)) yTmp = 0;
            }
            else
            {
                if ((col_flags == (BOT_LEFT_COL | TOP_LEFT_COL)) || (col_flags == (BOT_RIGHT_COL | TOP_RIGHT_COL)))
                {
                    if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
                    col_flags = check_collision(player.x, player.y + yColOffset);
                    if (handle_collision_v(yColOffset, col_flags)) yTmp = 0;
                }
                else
                {
                    if (handle_collision_v(yColOffset, col_flags)) yTmp = 0;
                    col_flags = check_collision(player.x + xColOffset, player.y);
                    if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
                }
            }
        }

        player.x += player.xSpd;
        player.y += player.ySpd;

        if (player.ySpd < -MAX_Y_SPEED_IN_SUBPIXELS)
        {
            player.ySpd += GRAVITY_CONST << 1;
            player.ySpd = MIN(-MAX_Y_SPEED_IN_SUBPIXELS, player.ySpd);
        }
        else
        {
            player.ySpd += GRAVITY_CONST;
            player.ySpd = MIN(MAX_Y_SPEED_IN_SUBPIXELS << 1, player.ySpd);
        }

        // decelerate Y and X
        if (player.grounded)
        {
            if (player.xSpd < X_GROUND_DECELERATION_IN_SUBPIXELS && player.xSpd > -X_GROUND_DECELERATION_IN_SUBPIXELS)
            {
                player.xSpd = 0;
            }
            else if (player.xSpd < -MAX_X_GROUND_SPEED_IN_SUBPIXELS)
            {
                player.xSpd += X_GROUND_DECELERATION_IN_SUBPIXELS << 1;
                player.xSpd = MIN(-MAX_X_GROUND_SPEED_IN_SUBPIXELS, player.xSpd);
            }
            else if (player.xSpd > MAX_X_GROUND_SPEED_IN_SUBPIXELS)
            {
                player.xSpd -= X_GROUND_DECELERATION_IN_SUBPIXELS << 1;
                player.xSpd = MAX(MAX_X_GROUND_SPEED_IN_SUBPIXELS, player.xSpd);
            }
            else if (player.xSpd < 0)
            {
                player.xSpd += X_GROUND_DECELERATION_IN_SUBPIXELS;
            }
            else if (player.xSpd)
            {
                player.xSpd -= X_GROUND_DECELERATION_IN_SUBPIXELS; 
            }
        }
        else
        {
            if (player.xSpd < X_DECELERATION_IN_SUBPIXELS && player.xSpd > -X_DECELERATION_IN_SUBPIXELS)
            {
                player.xSpd = 0;
            }
            else if (player.xSpd < -MAX_X_SPEED_IN_SUBPIXELS)
            {
                player.xSpd += X_DECELERATION_IN_SUBPIXELS << 1;
                player.xSpd = MIN(-MAX_X_SPEED_IN_SUBPIXELS, player.xSpd);
            }
            else if (player.xSpd > MAX_X_SPEED_IN_SUBPIXELS)
            {
                player.xSpd -= X_DECELERATION_IN_SUBPIXELS << 1;
                player.xSpd = MAX(MAX_X_SPEED_IN_SUBPIXELS, player.xSpd);
            }
            else if (player.xSpd < 0)
            {
                player.xSpd += X_DECELERATION_IN_SUBPIXELS;
            }
            else if (player.xSpd)
            {
                player.xSpd -= X_DECELERATION_IN_SUBPIXELS; 
            }

            if (player.animIndex != AIR_IDLE_ANIM_INDEX)
            {
                player.animIndex = AIR_IDLE_ANIM_INDEX;
                player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
                player.animSpeed = AIR_IDLE_ANIM_SPEED;
                player.animFrame = 0;
                player.fallDelay = 6;
            }
        }
    }
    // Hook physics
    else
    {
        uint8_t bPlayerInput = (player.flags & PF_HASINPUT) != 0;
        if (!bPlayerInput) {
            player.angularAcc = 0;
        }
        player.angularAcc += SIN(player.hookAngle) >> 1;

        if (player.angularAcc < 0)
        {
            player.angularAcc = -1 * player.angularAcc;
            player.angularAcc = player.angularAcc / (player.hookLength >> 2);
        }
        else
        {
            player.angularAcc = player.angularAcc / (player.hookLength >> 2);
            player.angularAcc = -1 * player.angularAcc;
        }

        if (player.angularVel < MIN_ANGULAR_VELOCITY + (player.hookLength >> 2))
        {
            player.angularVel += 2;
        }
        else if (player.angularVel > MAX_ANGULAR_VELOCITY - (player.hookLength >> 2))
        {
            player.angularVel -= 2;
        }
        else
        {
            player.angularVel += player.angularAcc;
        }

        // Taper off to angle 0 even if there is no more velocity
        // this ensures that we always settle down to angle 0 with no input
        // unless we are blocked by a wall, then we can't settle to 0
        int8_t angleSettleAlignment = 0;
        uint8_t oldAngle = player.hookAngle;
        int8_t angleAdjust = 0;
        if (player.angularVel >= 0)
        {
            angleAdjust = player.angularVel >> 4;
        }
        else
        {
            angleAdjust = -1 * ((-1 * player.angularVel) >> 4);
        }

        if (angleAdjust == 0)
        {
            if (player.hookAngle >= ANGLE_360DEG - 20)
            {
                player.hookAngle += 1;
                angleSettleAlignment = -1;
                //player.angularVel = 0;
            }
            else if (player.hookAngle > 0 && player.hookAngle <= 20)
            {
                player.hookAngle -= 1;
                angleSettleAlignment = 1;
                //player.angularVel = 0;
            }
        }
        else
        {
            player.hookAngle += angleAdjust;
        }

        // Hook length needs to be << 4 and sin(a) needs >> 7, so after mult it's >> 3
        int16_t xOffset = (player.hookLength * SIN(player.hookAngle)) >> 3;
        int16_t yOffset = (player.hookLength * COS(player.hookAngle)) >> 3;
        player.x = player.hookX + xOffset;
        player.y = player.hookY + yOffset;
        uint8_t col_flags = check_collision(player.x, player.y);
        uint8_t nAttempts = 12;
        int8_t tmpAngle = 0;
        int8_t angularVelSign = sign(player.angularVel);
        if (col_flags)
        {
            if (col_flags != player.oldColFlags && player.soundDelay == 0)
            {
                music_play_sfx(BANK(sfx_03), sfx_03, SFX_MUTE_MASK(sfx_03), MUSIC_SFX_PRIORITY_NORMAL);
                player.soundDelay = 60;
            }

            while (col_flags && nAttempts > 0)
            {
                if (angleSettleAlignment == 0 || player.oldHookLength != player.hookLength)
                {
                    if (angularVelSign < 0)
                    {
                        tmpAngle += 1;
                    }
                    else
                    {
                        tmpAngle -= 1;
                    }
                }
                else
                {
                    tmpAngle += angleSettleAlignment;
                }

                xOffset = (player.hookLength * SIN(player.hookAngle + tmpAngle)) >> 3;
                yOffset = (player.hookLength * COS(player.hookAngle + tmpAngle)) >> 3;
                player.x = player.hookX + xOffset;
                player.y = player.hookY + yOffset;
                col_flags = check_collision(player.x, player.y);
                nAttempts -= 1;
                // If we're adjusting hook length, check both directions for a valid path
                if (col_flags && (nAttempts & 1u) && player.oldHookLength != player.hookLength)
                {
                    tmpAngle = -tmpAngle + sign(tmpAngle);
                    angularVelSign = -angularVelSign;
                    if (angularVelSign == 0) angularVelSign = -1;
                }
            }

            // Failed to update due to collision
            if (nAttempts == 0)
            {
                player.hookAngle = oldAngle;
                if (player.oldHookLength != player.hookLength)
                {
                    player.hookLength = player.oldHookLength;
                    xOffset = (player.hookLength * SIN(player.hookAngle)) >> 3;
                    yOffset = (player.hookLength * COS(player.hookAngle)) >> 3;
                    player.x = player.hookX + xOffset;
                    player.y = player.hookY + yOffset;
                }
                else
                {
                    xOffset = (player.hookLength * SIN(player.hookAngle)) >> 3;
                    yOffset = (player.hookLength * COS(player.hookAngle)) >> 3;
                    handle_collision_h(xOffset, col_flags);
                    check_collision(player.x, player.y + yOffset);
                    handle_collision_v(yOffset, col_flags);
                }
            }
            else
            {
                player.hookAngle = player.hookAngle + tmpAngle;
                if (player.colFlags != player.oldColFlags)
                {
                    player.angularVel = -(player.angularVel >> 1);
                }
            }
        }

        player.oldColFlags = player.colFlags;

        // Decelerate, otherwise we'll swing back and forth forever
        if (!bPlayerInput)
        {
            if (player.angularVel < 0)
            {
                player.angularVel += 1;
            }
            else if (player.angularVel)
            {
                player.angularVel -= 1;
            }
        }
    }

    if (player.x != player.oldX || player.y != player.oldY)
    {
        // Update collision flags
        update_tilemap_collision(player.x, player.y);

        // lower the collision size for some leniency on the hurtbox
        collision_size_offset = 2;
        check_collision(player.x, player.y);
        collision_size_offset = 0;

        // Check special collision cases
        if (collision_botleft == COL_DEATH || collision_botright == COL_DEATH ||
            collision_topleft == COL_DEATH || collision_topright == COL_DEATH)
        {
            if ((player.flags & PF_HASATL) != 0)
            {
                music_play_sfx(BANK(sfx_09), sfx_09, SFX_MUTE_MASK(sfx_09), MUSIC_SFX_PRIORITY_NORMAL); 
                player.flags &= ~PF_HASATL;
                player.iFrames = 180;
                hide_atl();
            }
            else if (player.iFrames == 0)
            {
                music_play_sfx(BANK(sfx_09), sfx_09, SFX_MUTE_MASK(sfx_09), MUSIC_SFX_PRIORITY_NORMAL); 
                game.gameState = GS_FADE_OUT;
                game.nextState = GS_START_LEVEL;
                gfx.fade_delay = 12;
                gfx.fade_step_length = 2;
                if (game.deaths < 999)
                {
                    game.deaths += 1;
                }
                return;
            }
        }
        else if (collision_botleft == COL_BOOST_RIGHT || collision_topleft == COL_BOOST_RIGHT ||
            collision_botright == COL_BOOST_RIGHT || collision_topright == COL_BOOST_RIGHT)
        {
            if (player.hookState != HS_ATTACHED)
            {
                player.xSpd += 15;
            }
            else
            {
                player.angularVel += 15;
            }
        }
        else if (collision_botleft == COL_BOOST_UP || collision_topleft == COL_BOOST_UP ||
            collision_botright == COL_BOOST_UP || collision_topright == COL_BOOST_UP)
        {
            if (player.hookState != HS_ATTACHED)
            {
                player.ySpd -= 15;
            }
            else
            {
                player.angularVel += 15;
            }
        }

        if (((game.flags & GF_ITEM_PICKED) == 0) && ((player.flags & PF_HASKEY) == 0) &&
            (tile_botleft == KEY_BACKGROUND_TILE_INDEX || tile_topleft == KEY_BACKGROUND_TILE_INDEX ||
            tile_botright == KEY_BACKGROUND_TILE_INDEX || tile_topright == KEY_BACKGROUND_TILE_INDEX))
        {
            player.flags |= PF_HASKEY;
            game.flags |= GF_ITEM_PICKED;
            gfx.update_background = 1;
            key.x = player.x;
            key.y = player.y;
        }
        else if ((player.flags & PF_HASKEY) != 0)
        {
            if (tile_botleft == CLOSED_DOOR_TILE1_INDEX || tile_topleft == CLOSED_DOOR_TILE1_INDEX ||
                tile_botright == CLOSED_DOOR_TILE1_INDEX || tile_topright == CLOSED_DOOR_TILE1_INDEX)
            {
                player.flags &= ~PF_HASKEY;
                game.flags |= GF_DOOR_OPEN;
                gfx.update_background = 1;
                music_play_sfx(BANK(sfx_05), sfx_05, SFX_MUTE_MASK(sfx_05), MUSIC_SFX_PRIORITY_NORMAL);
                hide_key();
            }
        }

        if (((game.flags & GF_ITEM_PICKED) == 0) && ((player.flags & PF_HASATL) == 0) &&
            (tile_botleft == ATL_BACKGROUND_TILE_INDEX || tile_topleft == ATL_BACKGROUND_TILE_INDEX ||
            tile_botright == ATL_BACKGROUND_TILE_INDEX || tile_topright == ATL_BACKGROUND_TILE_INDEX))
        {
            player.flags |= PF_HASATL;
            game.flags |= GF_ITEM_PICKED;
            gfx.update_background = 1;
            atl.x = player.x;
            atl.y = player.y;
        }

        player.oldX = player.x;
        player.oldY = player.y;

        set_camera_target();
    }

    // Non player physics
    if ((player.flags & PF_HASKEY) != 0)
    {
        move_obj_to_player((basic_obj_t*)&key);
    }
    if ((player.flags & PF_HASATL) != 0)
    {
        move_obj_to_player((basic_obj_t*)&atl);
    }
}
