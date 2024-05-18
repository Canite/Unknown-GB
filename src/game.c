#include "../include/game.h"

struct Game game = {0};

void init_game(void)
{
    game.gameState = GS_TITLE_LOAD;
    game.nextState = GS_TITLE;
    game.gameFrame = 0;
    game.levelFrame = 0;
    game.currentLevel = 0;
    game.deaths = 0;
    game.level_data.tiles = 0;
    game.level_data.collisions = 0;
    game.level_data.tile_width = 0;
    game.level_data.tile_height = 0;
    game.mapPixelW = 0;
    game.mapPixelH = 0;
    game.mapX = 0;
    game.mapY = 0;
    game.oldMapX = 255;
    game.oldMapY = 255;
    game.flags = GF_NONE;
}

void game_loop(void)
{
    // Loop forever
    while(TRUE)
    {
        // Get new inputs
        old_joy = joy;
        joy = joypad();
        // Optimization / compiler bug(?) work-around: Store variable for (joy & last_joy)
        joy_pressed = joy;
        joy_pressed &= ~old_joy;
        joy_released = ~joy;
        joy_released &= ~old_joy;

        switch(game.gameState)
        {
            case GS_TITLE_LOAD:
                game.gameState = GS_TITLE;

            case GS_TITLE:
                process_title_input();

                break;
            case GS_START_GAME:
                gfx.draw_window = 1;
                start_music = 1;

            case GS_START_LEVEL:
                game.gameState = GS_FADE_IN;
                game.nextState = GS_INGAME;
                gfx.fade_delay = 30;
                gfx.fade_step_length = 5;
                start_level();
                update_game_sprites();
                if (start_music)
                {
                    start_music = 0;
                    music_load(BANK(bs_redsandMachine_quiet), &bs_redsandMachine_quiet), music_pause(music_paused = FALSE);
                }
                break;
            case GS_INGAME:
                process_game_input();
                apply_physics();
                update_camera_coordinates();
                update_game_sprites();
                update_window();
                update_background();
                game.levelFrame += 1;

                break;
            case GS_PAUSE:
                process_pause_input();

                break;
            case GS_FADE_OUT:
                fade_out();

                break;
            case GS_FADE_IN:
                fade_in();

                break;

            case GS_END:
                draw_end_screen();
                break;
            default:
                break;
        }

        game.gameFrame += 1;

        // Done processing, yield CPU and wait for start of next frame
        vsync();

        // update camera after vblank
        update_camera();
    }
}

void start_level(void)
{
    if (game.currentLevel > NUM_LEVELS)
    {
        for (int i = 0; i < 32; i++)
            hide_sprite(i);

        init_end_screen();
        init_camera();
        camera.redraw = 1;
        game.nextState = GS_END;
        gfx.update_background = 1;
        gfx.draw_window = 0;
    }
    else
    {
        init_level(game.currentLevel);
    }
}
