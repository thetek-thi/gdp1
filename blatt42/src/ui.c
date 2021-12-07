#include "types.h"
#include "ui.h"

#include "color.h"

#include <curses.h>



void
ui_draw_frame ()
{
    attron (COLOR_PAIR (COLP_GREY));

    mvprintw (0,         0,        "╭");
    mvprintw (0,         COLS - 1, "╮");
    mvprintw (LINES - 2, 0,        "╰");
    mvprintw (LINES - 2, COLS - 1, "╯");

    for (int i = 1; i < COLS-1; i++)
    {
        mvprintw (0,         i, "─");
        mvprintw (LINES - 2, i, "─");
    }

    for (int i = 1; i < LINES-2; i++)
    {
        mvprintw (i, 0,      "│");
        mvprintw (i, COLS-1, "│");
    }

    attroff (COLOR_PAIR (COLP_GREY));
}



void
ui_update_msg (worm_t *worm)
{
    attron (COLOR_PAIR (COLP_ORANGE));
    mvprintw (LINES-1, 1, "POS x    y      LEN");
    attroff (COLOR_PAIR (COLP_ORANGE));

    attron (COLOR_PAIR (COLP_FG));
    pos_t headpos = worm->pos[worm->headindex];
    mvprintw (LINES-1,  7, "%02d", headpos.x);
    mvprintw (LINES-1, 12, "%02d", headpos.y);

    mvprintw (LINES-1, 21, "%2d", worm->len_pref);
    attroff (COLOR_PAIR (COLP_FG));
}



void
ui_draw_food (food_t food[])
{
    for (int i = 0; i < FOOD_COUNT; i++)
    {
        colorpair_t color;
        switch (food[i].value)
        {
            case 2: color = COLP_BLUE;   break;
            case 4: color = COLP_VIOLET; break;
            case 6: color = COLP_PURPLE; break;
        }

        attron (COLOR_PAIR (color));
        mvprintw (food[i].pos.y + 1, (food[i].pos.x * 2) + 1, "██");
        attroff (COLOR_PAIR (color));
    }
}

