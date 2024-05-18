#ifndef _INPUT_H
#define _INPUT_H

#include <stdint.h>
//#include <gb/gb.h>
#include <gbdk/platform.h>

#include "game.h"
#include "physics.h"
#include "player.h"
#include "utils.h"
#include "sounds.h"
#include "../res/sfx.h"

// new and previous values of the joypad input
extern uint8_t joy;
extern uint8_t old_joy;
extern uint8_t joy_pressed;
extern uint8_t joy_released;

// check button down
#define INPUT_KEY(key) (joy&(key))
// check button press
#define INPUT_KEYPRESS(key) (joy_pressed & (key))
// check button release
#define INPUT_KEYRELEASE(key) (joy_released & (key))

void process_title_input(void) BANKED;
BANKREF_EXTERN(process_title_input)
void process_game_input(void) BANKED;
BANKREF_EXTERN(process_game_input)
void process_pause_input(void) BANKED;
BANKREF_EXTERN(process_pause_input)

#endif
