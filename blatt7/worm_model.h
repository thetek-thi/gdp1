#include "worm.h"



#ifndef _WORM_MODEL_H
#define _WORM_MODEL_H


rescode_t init_worm        (int head_y, int head_x, worm_heading_t dir, color_pair_t color);
void      show_worm        ();
void      move_worm        (game_state_t* state);
void      set_worm_heading (worm_heading_t dir);


#endif

