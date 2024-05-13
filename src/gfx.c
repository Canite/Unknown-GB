#include "../include/gfx.h"

struct Camera camera = {0};
struct WindowData window = {0};
struct GraphicsInfo gfx = {0};

void init_gfx(void)
{
    gfx.fade_delay = 0;
    gfx.fade_step_length = 0;
    gfx.sprites_inited = 0;
    gfx.draw_window = 0;
    gfx.update_background = 0;

    init_camera();
    init_window();
}

void init_camera(void)
{
    camera.x = 0;
    camera.y = 0;
    camera.oldX = 0;
    camera.oldY = 0;
    camera.targetX = 0;
    camera.targetY = 0;
    camera.xSpd = 0;
    camera.ySpd = 0;
    camera.redraw = 0;
}

void init_window(void)
{
    window.timer_frames = 0;
    window.timer_minutes = 0;
    window.timer_seconds = 0;
    window.drawn_deaths = 1000;
    window.drawn_level = 255;
    window.death_counter_tiles[0] = FONT_SKULL_TILE_INDEX;
    window.death_counter_tiles[1] = FONT_START_TILE_INDEX;
    window.death_counter_tiles[2] = FONT_START_TILE_INDEX;
    window.death_counter_tiles[3] = FONT_START_TILE_INDEX;
    window.level_timer_tiles[0] = FONT_CLOCK_TILE_INDEX;
    window.level_timer_tiles[1] = FONT_START_TILE_INDEX;
    window.level_timer_tiles[2] = FONT_START_TILE_INDEX;
    window.level_timer_tiles[3] = FONT_COLON_TILE_INDEX;
    window.level_timer_tiles[4] = FONT_START_TILE_INDEX;
    window.level_timer_tiles[5] = FONT_START_TILE_INDEX;
    window.level_counter_tiles[0] = FONT_LV_TILE_INDEX;
    window.level_counter_tiles[1] = FONT_START_TILE_INDEX;
    window.level_counter_tiles[2] = FONT_START_TILE_INDEX;

    // hidden until pause is pressed
    uint8_t pause_tiles[3];
    pause_tiles[0] = FONT_PAUSE_TILE_INDEX;
    pause_tiles[1] = FONT_PAUSE_TILE_INDEX + 1;
    pause_tiles[2] = FONT_PAUSE_TILE_INDEX + 2;
    set_win_tiles(8, 1, 3, 1, pause_tiles);
}

void clear_background(void)
{
    for (uint16_t i = 0; i < 20; i ++)
    {
        for (uint16_t j = 0; j < 18; j ++)
        {
            set_tile_xy(i, j, BLANK_TILE_INDEX);
        }
    }
}

// Fade is in 6 steps, so step length should be set to delay / 6
void fade_out(void)
{
    if (gfx.fade_delay != 0)
    {
        if (gfx.fade_delay == (gfx.fade_step_length * 5))
        {
            BGP_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY);
            OBP1_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        }
        else if (gfx.fade_delay == (gfx.fade_step_length * 4))
        {
            BGP_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY);
            OBP1_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        }
        else if (gfx.fade_delay == (gfx.fade_step_length * 3))
        {
            BGP_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY);
            OBP1_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        }
        else if (gfx.fade_delay == (gfx.fade_step_length * 2))
        {
            BGP_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP1_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
        }
        else if (gfx.fade_delay == gfx.fade_step_length)
        {
            BGP_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
            OBP1_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK, DMG_BLACK);
        }
        gfx.fade_delay -= 1;
    }
    else
    {
        BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
        OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
        OBP1_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
        HIDE_SPRITES;
        game.gameState = game.nextState;
        gfx.fade_step_length = 0;
    }
}

void fade_in(void)
{
    if (gfx.fade_delay != 0)
    {
        if (gfx.fade_delay == (gfx.fade_step_length * 5))
        {
            BGP_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
            OBP1_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK, DMG_BLACK);
        }
        else if (gfx.fade_delay == (gfx.fade_step_length * 4))
        {
            BGP_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP1_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
        }
        else if (gfx.fade_delay == (gfx.fade_step_length * 3))
        {
            BGP_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY);
            OBP1_REG = DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        }
        else if (gfx.fade_delay == (gfx.fade_step_length * 2))
        {
            BGP_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY);
            OBP1_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        }
        else if (gfx.fade_delay == gfx.fade_step_length)
        {
            BGP_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY);
            OBP1_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        }
        gfx.fade_delay -= 1;
    }
    else
    {
        BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY);
        OBP1_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
        SHOW_SPRITES;
        game.gameState = game.nextState;
        gfx.fade_step_length = 0;
    }
}

void update_window(void)
{
    if (window.timer_frames == 0 || window.timer_frames >= 60)
    {
        // time updated by 1 second, recalc and draw
        if (window.timer_seconds < 59)
        {
            // start timer at 00:00
            if (window.timer_frames != 0)
            {
                window.timer_seconds += 1;
            }
        }
        else
        {
            window.timer_seconds = 0;
            window.timer_minutes += 1;
        }

        // minutes
        window.level_timer_tiles[1] = FONT_START_TILE_INDEX;
        window.level_timer_tiles[2] = FONT_START_TILE_INDEX;
        uint8_t tmp_minutes = window.timer_minutes;
        while (tmp_minutes >= 10)
        {
            tmp_minutes -= 10;
            window.level_timer_tiles[1] += 1;
        }
        window.level_timer_tiles[2] = FONT_START_TILE_INDEX + tmp_minutes;

        // seconds
        window.level_timer_tiles[4] = FONT_START_TILE_INDEX;
        window.level_timer_tiles[5] = FONT_START_TILE_INDEX;
        uint8_t tmp_seconds = window.timer_seconds;
        while (tmp_seconds >= 10)
        {
            tmp_seconds -= 10;
            window.level_timer_tiles[4] += 1;
        }
        window.level_timer_tiles[5] = FONT_START_TILE_INDEX + tmp_seconds;

        set_win_tiles(14, 0, 6, 1, window.level_timer_tiles);
        window.timer_frames = 0;
    }

    if (window.timer_minutes < 99)
    {
        window.timer_frames += 1;
    }

    // death counter
    if (window.drawn_deaths != game.deaths || window.drawn_deaths == 1000)
    {
        window.drawn_deaths = game.deaths;

        window.death_counter_tiles[1] = FONT_START_TILE_INDEX;
        window.death_counter_tiles[2] = FONT_START_TILE_INDEX;
        window.death_counter_tiles[3] = FONT_START_TILE_INDEX;

        uint16_t tmp_deaths = window.drawn_deaths;
        while (tmp_deaths >= 100)
        {
            tmp_deaths -= 100;
            window.death_counter_tiles[1] += 1;
        }

        while (tmp_deaths >= 10)
        {
            tmp_deaths -= 10;
            window.death_counter_tiles[2] += 1;
        }

        window.death_counter_tiles[3] = FONT_START_TILE_INDEX + tmp_deaths;
        set_win_tiles(0, 0, 4, 1, window.death_counter_tiles);
    }

    // level counter
    if (window.drawn_level != game.currentLevel || window.drawn_level == 255)
    {
        window.drawn_level = game.currentLevel;

        window.level_counter_tiles[1] = FONT_START_TILE_INDEX;
        window.level_counter_tiles[2] = FONT_START_TILE_INDEX;

        uint8_t tmp_level = window.drawn_level;

        while (tmp_level >= 10)
        {
            tmp_level -= 10;
            window.level_counter_tiles[1] += 1;
        }

        window.level_counter_tiles[2] = FONT_START_TILE_INDEX + tmp_level;
        set_win_tiles(8, 0, 3, 1, window.level_counter_tiles);
    }
}

void update_camera(void) NONBANKED
{
    uint8_t currentBank = CURRENT_BANK;
    SWITCH_ROM(game.level_data.tiles_bank);

    if (camera.redraw)
    {
        camera.redraw = 0;

        // update hardware scroll position
        SCY_REG = camera.y; SCX_REG = camera.x; 
        // up or down
        game.mapY = (uint8_t)(camera.y >> 3u);
        if (game.mapY != game.oldMapY)
        {
            if (camera.y < camera.oldY)
            {
                set_bkg_submap(game.mapX, game.mapY, MIN(21u, game.level_data.tile_width-game.mapX), 1, game.level_data.tiles, game.level_data.tile_width);
            }
            else
            {
                if ((game.level_data.tile_height - 18u) > game.mapY) set_bkg_submap(game.mapX, game.mapY + 18u, MIN(21u, game.level_data.tile_width-game.mapX), 1, game.level_data.tiles, game.level_data.tile_width);
            }
            game.oldMapY = game.mapY; 
        }

        // left or right
        game.mapX = (uint8_t)(camera.x >> 3u);
        if (game.mapX != game.oldMapX)
        {
            if (camera.x < camera.oldX)
            {
                set_bkg_submap(game.mapX, game.mapY, 1, MIN(19u, game.level_data.tile_height - game.mapY), game.level_data.tiles, game.level_data.tile_width);
            }
            else
            {
                if ((game.level_data.tile_width - 20u) > game.mapX) set_bkg_submap(game.mapX + 20u, game.mapY, 1, MIN(19u, game.level_data.tile_height - game.mapY), game.level_data.tiles, game.level_data.tile_width);
            }
            game.oldMapX = game.mapX;
        }

        // set old camera position to current camera position
        camera.oldX = camera.x;
        camera.oldY = camera.y;
    }

    SWITCH_ROM(currentBank);
}

void set_camera_target(void)
{
    uint16_t playerPixelX = SUBPIXELS_TO_PIXELS(player.x);
    uint16_t playerPixelY = SUBPIXELS_TO_PIXELS(player.y);

    if (playerPixelX > 80) camera.targetX = playerPixelX - 80;
    if (playerPixelY > 84) camera.targetY = playerPixelY - 84;
    if (camera.targetX > ((game.level_data.tile_width << 3) - 160)) camera.targetX = ((game.level_data.tile_width << 3) - 160);
    if (camera.targetY > ((game.level_data.tile_height << 3) - 144)) camera.targetY = ((game.level_data.tile_height << 3) - 144);
}

void update_camera_coordinates(void)
{
    if (camera.x != camera.targetX || camera.y != camera.targetY)
    {
        // tried to do some lerp on the camera but it ends being headachey
        /*
        if (camera.x > camera.targetX)
        {
            camera.xSpd = -((camera.x - camera.targetX) >> 2);
        }
        else 
        {
            camera.xSpd = (camera.targetX - camera.x) >> 2;
        }

        if (camera.y > camera.targetY)
        {
            camera.ySpd = -((camera.y - camera.targetY) >> 2);
        }
        else 
        {
            camera.ySpd = (camera.targetY - camera.y) >> 2;
        }

        if (camera.xSpd == 0) camera.xSpd = sign(camera.targetX - camera.x);
        if (camera.ySpd == 0) camera.ySpd = sign(camera.targetY - camera.y);

        camera.x += camera.xSpd;
        camera.y += camera.ySpd;
        */
        camera.x = camera.targetX;
        camera.y = camera.targetY;
        camera.redraw = 1;
    }
}

void update_game_sprites(void)
{
    // Player sprite start
    set_sprite_tile(PLAYER_SPRITE_INDEX, player.animIndex + player.animFrame);
    if ((game.gameFrame & player.animSpeed) == player.animSpeed)
    {
        if (player.animFrame < player.numAnimFrames - 1)
        {
            player.animFrame += 1;
        }
        else
        {
            player.animFrame = 0;
        }
    }

    int16_t playerPixelX = SUBPIXELS_TO_PIXELS(player.x) - camera.x;
    int16_t playerPixelY = SUBPIXELS_TO_PIXELS(player.y) - camera.y;

    if ((player.iFrames & 7u) == 7u)
    {
        hide_sprite(PLAYER_SPRITE_INDEX);
    }
    else
    {
        if (player.facing)
        {
            set_sprite_prop(PLAYER_SPRITE_INDEX, S_FLIPX);
            move_sprite(PLAYER_SPRITE_INDEX, playerPixelX, playerPixelY);
        }
        else
        {
            set_sprite_prop(PLAYER_SPRITE_INDEX, 0);
            move_sprite(PLAYER_SPRITE_INDEX, playerPixelX, playerPixelY);
        }
    }

    if (player.hookState == HS_ATTACHED)
    {
        draw_hook();
    }
    else
    {
        hide_hook();
        //draw_hook_indicator();
    }

    // Player sprite end

    // TODO: eventually turn this into some list stored on the game struct
    if ((player.flags & PF_HASKEY) != 0)
    {
        int16_t keyPixelX = SUBPIXELS_TO_PIXELS(key.x) - camera.x;
        int16_t keyPixelY = SUBPIXELS_TO_PIXELS(key.y) - camera.y;

        move_sprite(KEY_SPRITE_INDEX, keyPixelX, keyPixelY);
    }

    if ((player.flags & PF_HASATL) != 0)
    {
        int16_t atlPixelX = SUBPIXELS_TO_PIXELS(atl.x) - camera.x;
        int16_t atlPixelY = SUBPIXELS_TO_PIXELS(atl.y) - camera.y;

        move_sprite(ATL_SPRITE_INDEX, atlPixelX, atlPixelY);
    }

}

void update_background(void)
{
    if (gfx.update_background)
    {
        gfx.update_background = 0;
        uint8_t currentBank = CURRENT_BANK;
        if ((player.flags & PF_HASKEY) != 0)
        {
            SWITCH_ROM(BANK(caverns));
            set_bkg_data(KEY_BACKGROUND_TILE_INDEX, 1, caverns_tiles);
            SWITCH_ROM(currentBank);
        }

        if ((player.flags & PF_HASATL) != 0)
        {
            SWITCH_ROM(BANK(caverns));
            set_bkg_data(ATL_BACKGROUND_TILE_INDEX, 1, caverns_tiles);
            SWITCH_ROM(currentBank);
        }

        if ((game.flags & GF_DOOR_OPEN) != 0)
        {
            SWITCH_ROM(BANK(caverns));
            set_bkg_data(CLOSED_DOOR_TILE1_INDEX, 1, &caverns_tiles[OPEN_DOOR_TILE1_INDEX * 16]);
            set_bkg_data(CLOSED_DOOR_TILE2_INDEX, 1, &caverns_tiles[OPEN_DOOR_TILE2_INDEX * 16]);
            SWITCH_ROM(currentBank);
        }
    }
}


void draw_end_screen(void)
{
    if (gfx.update_background)
    {
        gfx.update_background = 0;
        hide_sprite(PLAYER_SPRITE_INDEX);
        set_bkg_tiles(8, 11, 5, 1, &window.level_timer_tiles[1]);
        set_bkg_tiles(8, 8, 3, 1, &window.death_counter_tiles[1]);
    }
}

void draw_hook(void)
{
    int8_t xOffset = (player.hookAngle) >> 2;
    int8_t yOffset = 8;
    if (player.hookAngle > ANGLE_315DEG)
    {
        xOffset = (player.hookAngle - ANGLE_360DEG) >> 2;
        yOffset = 8;
    }
    else if (player.hookAngle > ANGLE_270DEG)
    {
        xOffset = -8;
        yOffset = (player.hookAngle - ANGLE_270DEG) >> 2;
    }
    else if (player.hookAngle > ANGLE_225DEG)
    {
        xOffset = -8;
        yOffset = (player.hookAngle - ANGLE_270DEG) >> 2;
    }
    else if (player.hookAngle > ANGLE_180DEG)
    {
        xOffset = (ANGLE_180DEG - player.hookAngle) >> 2;
        yOffset = -8;
    }
    else if (player.hookAngle > ANGLE_135DEG)
    {
        xOffset = (ANGLE_180DEG - player.hookAngle) >> 2;
        yOffset = -8;
    }
    else if (player.hookAngle > ANGLE_90DEG)
    {
        xOffset = 8;
        yOffset = (ANGLE_90DEG - player.hookAngle) >> 2;
    }
    else if (player.hookAngle > ANGLE_45DEG)
    {
        xOffset = 8;
        yOffset = (ANGLE_90DEG - player.hookAngle) >> 2;
    }

    uint16_t hookPixelX = SUBPIXELS_TO_PIXELS(player.hookX) - camera.x;
    uint16_t hookPixelY = SUBPIXELS_TO_PIXELS(player.hookY) - camera.y;
    for (uint8_t i = 0; i < player.hookSegments; i++)
    {
        set_sprite_tile(i + 1, HOOK_SPRITE_TILE_INDEX);
        move_sprite(i + 1, hookPixelX + (xOffset * i), hookPixelY + (yOffset * i));
    }

    uint8_t totalOffset = (abs8(xOffset) + abs8(yOffset)) * player.hookSegments;
    uint8_t hiddenSegments = 1;
    while (totalOffset > 70)
    {
        hide_sprite(HOOK_SPRITE_INDEX + player.hookSegments - hiddenSegments);
        totalOffset -= 70;
        hiddenSegments += 1;
    }
}

inline void hide_key(void)
{
    hide_sprite(KEY_SPRITE_INDEX);
}

inline void hide_atl(void)
{
    hide_sprite(ATL_SPRITE_INDEX);
}

inline void hide_hook(void)
{
    for (uint8_t i = 0; i < player.hookSegments; i++)
    {
        hide_sprite(HOOK_SPRITE_INDEX + i);
    }

    player.hookSegments = 0;
}

void draw_hook_indicator(void)
{
    if ((game.gameFrame & 31u) == 31u)
    {
        int16_t xCheck = PIXELS_TO_SUBPIXELS(4);
        int16_t yCheck = -PIXELS_TO_SUBPIXELS(4);
        uint16_t maxDist = MAX_HOOK_DISTANCE;

        if (player.lookState == LS_UP)
        {
            xCheck = 0;
            yCheck = -PIXELS_TO_SUBPIXELS(8);
            maxDist = MAX_STRAIGHT_HOOK_DISTANCE;
        }
        else if (player.facing)
        {
            xCheck = -PIXELS_TO_SUBPIXELS(4);
        }

        uint8_t col_flags = 0;
        int16_t xTmp = 0;
        int16_t yTmp = 0;

        while (!col_flags && ((abs16(xTmp) + abs16(yTmp)) < maxDist))
        {
            xTmp += xCheck;
            yTmp += yCheck;
            col_flags = check_collision(player.x + xTmp, player.y + yTmp);
        }

        if ((abs16(xTmp) + abs16(yTmp)) >= maxDist || (abs16(xTmp) + abs16(yTmp)) <= MIN_HOOK_DISTANCE)
        {
            hide_sprite(HOOK_INDICATOR_SPRITE_INDEX);
        }
        else
        {
            uint16_t indicatorX = SUBPIXELS_TO_PIXELS((player.x + xTmp)) - camera.x;
            uint16_t indicatorY = SUBPIXELS_TO_PIXELS((player.y + yTmp)) - camera.y;

            set_sprite_tile(HOOK_INDICATOR_SPRITE_INDEX, HOOK_INDICATOR_SPRITE_TILE_INDEX);
            move_sprite(HOOK_INDICATOR_SPRITE_INDEX, indicatorX, indicatorY);
        }
    }
    else if (((game.gameFrame + 16) & 31u) == 31u)
    {
        hide_sprite(HOOK_INDICATOR_SPRITE_INDEX);
    }
}
