#include "../include/physics.h"

const int8_t sine_wave[256] = {
    0,3,6,9,12,16,19,22,25,28,31,34,37,40,43,46,49,51,54,57,60,
    63,65,68,71,73,76,78,81,83,85,88,90,92,94,96,98,100,102,104,
    106,107,109,111,112,113,115,116,117,118,120,121,122,122,123,
    124,125,125,126,126,126,127,127,127,127,127,127,127,126,126,
    126,125,125,124,123,122,122,121,120,118,117,116,115,113,112,
    111,109,107,106,104,102,100,98,96,94,92,90,88,85,83,81,78,76,
    73,71,68,65,63,60,57,54,51,49,46,43,40,37,34,31,28,25,22,19,
    16,12,9,6,3,0,-3,-6,-9,-12,-16,-19,-22,-25,-28,-31,-34,-37,
    -40,-43,-46,-49,-51,-54,-57,-60,-63,-65,-68,-71,-73,-76,-78,
    -81,-83,-85,-88,-90,-92,-94,-96,-98,-100,-102,-104,-106,-107,
    -109,-111,-112,-113,-115,-116,-117,-118,-120,-121,-122,-122,
    -123,-124,-125,-125,-126,-126,-126,-127,-127,-127,-127,-127,
    -127,-127,-126,-126,-126,-125,-125,-124,-123,-122,-122,-121,
    -120,-118,-117,-116,-115,-113,-112,-111,-109,-107,-106,-104,
    -102,-100,-98,-96,-94,-92,-90,-88,-85,-83,-81,-78,-76,-73,-71,
    -68,-65,-63,-60,-57,-54,-51,-49,-46,-43,-40,-37,-34,-31,-28,
    -25,-22,-19,-16,-12,-9,-6,-3
};

uint16_t isqrt(uint16_t x) NONBANKED
{
    uint16_t m, y, b;
    m = 0x4000;
    y = 0;
    while (m != 0) {
        b = y | m;
        y >>= 1;
        if (x >= b) {
            x -= b;
            y |= m;
        }
        m >>= 2;
    }
    return y;
}

void apply_physics(void)
{
    if (player.fallDelay)
    {
        player.fallDelay -= 1;
    }

    // Unhooked physics
    if (player.hookState != HS_ATTACHED)
    {
        int8_t xSpdSign = sign(player.xSpd);
        int8_t ySpdSign = sign(player.ySpd);
        int16_t xTmp = 0;
        int16_t yTmp = 0;
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

        player.ySpd += GRAVITY_CONST;

        // decelerate Y and X
        if (player.grounded)
        {
            if (player.xSpd < X_GROUND_DECELERATION_IN_SUBPIXELS && player.xSpd > -X_GROUND_DECELERATION_IN_SUBPIXELS)
            {
                player.xSpd = 0;
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
        player.angularAcc = SIN(player.hookAngle);
        if (player.angularAcc < 0)
        {
            player.angularAcc = -1 * player.angularAcc;
            player.angularAcc = player.angularAcc >> (player.hookSegments >> 1);
        }
        else
        {
            player.angularAcc = player.angularAcc >> (player.hookSegments >> 1);
            player.angularAcc = -1 * player.angularAcc;
        }

        if (bPlayerInput && sign(player.angularAcc) != sign(player.angularVel))
        {
            player.angularAcc = player.angularAcc >> 1;
        }

        player.angularVel += player.angularAcc;
        player.angularVel = CLAMP(player.angularVel, MIN_ANGULAR_VELOCITY + (player.hookSegments << 1), MAX_ANGULAR_VELOCITY - (player.hookSegments << 1));

        // Taper off to angle 0 even if there is no more velocity
        // this ensures that we always settle down to angle 0 with no input
        // unless we are blocked by a wall, then we can't settle to 0
        int8_t angleSettleAlignment = 0;
        uint8_t oldAngle = player.hookAngle;
        if (player.angularVel > 0)
        {
            if (player.angularVel >> 4 == 0 && player.hookAngle > 0 && player.hookAngle <= 9)
            {
                player.hookAngle -= 1;
                angleSettleAlignment = 1;
                player.angularVel = 0;
            }
            else
            {
                player.hookAngle += (player.angularVel) >> 4;
            }
        }
        else
        {
            if ((-1 * player.angularVel) >> 4 == 0  && player.hookAngle >= 246 /* && player.hookAngle <= 255 //always true */)
            {
                player.hookAngle += 1;
                angleSettleAlignment = -1;
                player.angularVel = 0;
            }
            else
            {
                player.hookAngle -= (-1 * player.angularVel) >> 4;
            }
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
                if (player.oldHookLength != player.hookLength)
                {
                    player.hookLength = player.oldHookLength;
                }
                player.hookAngle = oldAngle;
                xOffset = (player.hookLength * SIN(player.hookAngle)) >> 3;
                yOffset = (player.hookLength * COS(player.hookAngle)) >> 3;
                handle_collision_h(xOffset, col_flags);
                check_collision(player.x, player.y + yOffset);
                handle_collision_v(yOffset, col_flags);
                //player.x = player.hookX + xOffset;
                //player.y = player.hookY + yOffset;
            }
            else
            {
                player.hookAngle = player.hookAngle + tmpAngle;
                player.angularVel = -(player.angularVel >> 3);
            }
        }

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
        check_collision(player.x, player.y);

        // Check special collision cases
        if (collision_botleft == COL_DEATH || collision_topleft == COL_DEATH ||
            collision_botright == COL_DEATH || collision_topright == COL_DEATH)
        {
            start_level(game.currentLevel);
            return;
        }

        if ((player.flags & PF_HASKEY) == 0 &&
            (tile_botleft == KEY_BACKGROUND_TILE_INDEX || tile_topleft == KEY_BACKGROUND_TILE_INDEX ||
            tile_botright == KEY_BACKGROUND_TILE_INDEX || tile_topright == KEY_BACKGROUND_TILE_INDEX))
        {
            player.flags |= PF_HASKEY;
            set_bkg_data(KEY_BACKGROUND_TILE_INDEX, 1, caverns_tiles);
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
                set_bkg_data(CLOSED_DOOR_TILE1_INDEX, 1, &caverns_tiles[OPEN_DOOR_TILE1_INDEX * 16]);
                set_bkg_data(CLOSED_DOOR_TILE2_INDEX, 1, &caverns_tiles[OPEN_DOOR_TILE2_INDEX * 16]);
                hide_key();
            }
        }

        uint16_t playerPixelX = SUBPIXELS_TO_PIXELS(player.x);
        uint16_t playerPixelY = SUBPIXELS_TO_PIXELS(player.y);

        player.oldX = player.x;
        player.oldY = player.y;

        if (playerPixelX > 76) camera.x = playerPixelX - 76;
        if (playerPixelY > 144) camera.y = playerPixelY - 144;
        if (camera.x > ((game.level_data.tile_width << 3) - 160)) camera.x = ((game.level_data.tile_width << 3) - 160);
        if (camera.y > ((game.level_data.tile_height << 3) - 160)) camera.y = ((game.level_data.tile_height << 3) - 160);
        camera.redraw = 1;
    }

    // Non player physics
    if ((player.flags & PF_HASKEY) != 0)
    {
        uint16_t keyTargetX = player.x;
        uint16_t keyTargetY = player.y - 16;

        if (player.facing)
        {
            keyTargetX += 112;
        }
        else 
        {
            keyTargetX -= 112;
        }

        if (key.x != keyTargetX || key.y != keyTargetY)
        {
            if (key.x > keyTargetX)
            {
                key.xSpd = -((key.x - keyTargetX) >> 3);
            }
            else 
            {
                key.xSpd = (keyTargetX - key.x) >> 3;
            }

            if (key.y > keyTargetY)
            {
                key.ySpd = -((key.y - keyTargetY) >> 3);
            }
            else 
            {
                key.ySpd = (keyTargetY - key.y) >> 3;
            }

            key.x += key.xSpd;
            key.y += key.ySpd;
        }
    }
}
