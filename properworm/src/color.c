#include <curses.h>

#include "color.h"



void
setup_color_pairs ()
{
    init_pair (COLP_ACCENT, 72, 0);
    init_pair (COLP_GREY,    8, 0);
}

