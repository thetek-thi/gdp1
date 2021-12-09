#include "color.h"

#include <curses.h>



void
colors_init ()
{
    start_color ();

    init_color (COL_BG,     67,  71, 114);
    init_color (COL_BLUE,   443, 600, 933);
    init_color (COL_FG,     627, 659, 804);
    init_color (COL_GREY,   267, 294, 416);
    init_color (COL_GREEN,  584, 773, 380);
    init_color (COL_ORANGE, 965, 584, 357);
    init_color (COL_PURPLE, 890, 490, 886);
    init_color (COL_RED,    933, 427, 522);
    init_color (COL_VIOLET, 643, 522, 867);

    init_pair (COLP_BLUE,   COL_BLUE,   COL_BG);
    init_pair (COLP_FG,     COL_FG,     COL_BG);
    init_pair (COLP_GREY,   COL_GREY,   COL_BG);
    init_pair (COLP_GREEN,  COL_GREEN,  COL_BG);
    init_pair (COLP_ORANGE, COL_ORANGE, COL_BG);
    init_pair (COLP_PURPLE, COL_PURPLE, COL_BG);
    init_pair (COLP_RED,    COL_RED,    COL_BG);
    init_pair (COLP_VIOLET, COL_VIOLET, COL_BG);
}

