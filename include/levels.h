#ifndef _LEVELS_H
#define _LEVELS_H

#include <stdint.h>
#include <gb/gb.h>
#include <gbdk/platform.h>

#include "structs.h"
#include "game.h"
#include "key_obj.h"
#include "atl_obj.h"
#include "gfx.h"
#include "../res/caverns.h"
#include "../res/title.h"
#include "../res/end.h"
#include "../res/title_screen.h"
#include "../res/end_screen.h"
#include "../res/level1.h"
#include "../res/level2.h"
#include "../res/level3.h"
#include "../res/level4.h"
#include "../res/level5.h"
#include "../res/level6.h"
#include "../res/level7.h"
#include "../res/level8.h"
#include "../res/level9.h"
#include "../res/level10.h"
#include "../res/level11.h"
#include "../res/level12.h"
#include "../res/level13.h"
#include "../res/level14.h"
#include "../res/level15.h"
#include "../res/level16.h"

#define NUM_LEVELS 16

extern const level_t levels[];

void init_title(void);
void init_end_screen(void);
void init_level(uint8_t level);

#endif
