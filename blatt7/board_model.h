#include "worm.h"

#include <curses.h>



#ifndef _BOARD_MODEL_H
#define _BOARD_MODEL_H


typedef struct _vec {
    int x;
    int y;
} vec;


void place_item   (int y, int x, chtype symbol, color_pair_t color);
int  get_last_row ();
int  get_last_col ();


#endif

