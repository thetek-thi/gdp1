#include "board_model.h"

#include "worm.h"



// place item on board
void
place_item (int          y,
            int          x,
            chtype       symbol,
            color_pair_t color)
{
    move (y, x);
    attron (COLOR_PAIR (color));
    addch (symbol);
    attroff (COLOR_PAIR (color));
}



// getters

int get_last_row () { return LINES-1; }
int get_last_col () { return COLS-1;  }

