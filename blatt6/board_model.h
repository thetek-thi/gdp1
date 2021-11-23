#include "worm.h"

#include <curses.h>



#ifndef _BOARD_MODEL_H
#define _BOARD_MODEL_H

void place_item   (int y, int x, chtype symbol, color_pair_t color);
int  get_last_row ();
int  get_last_col ();

#endif

