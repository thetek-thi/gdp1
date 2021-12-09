#include "types.h"
#include "ui.h"

#include "color.h"
#include "game.h"

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
ui_update_msg (worm_t *worm, food_t food[], exitmsg_t exitmsg)
{
    if (exitmsg == EXITMSG_NONE)
    {
        attron (COLOR_PAIR (COLP_ORANGE));
        mvprintw (LINES-1, 1, "POS x    y       LEN       FOOD");
        attroff (COLOR_PAIR (COLP_ORANGE));

        attron (COLOR_PAIR (COLP_FG));
        pos_t headpos = worm->pos[worm->headindex];
        mvprintw (LINES-1,  7, "%02d", headpos.x);
        mvprintw (LINES-1, 12, "%02d", headpos.y);

        mvprintw (LINES-1, 22, "%2d", worm->len_pref);

        mvprintw (LINES-1, 33, "%2d", food_count (food));

        attroff (COLOR_PAIR (COLP_FG));
    }
    else if (exitmsg == EXITMSG_SUCCESS)
    {
        attron (COLOR_PAIR (COLP_GREEN));
        mvprintw (LINES -1, 1, "You won! Press any key to continue.");
        attroff (COLOR_PAIR (COLP_GREEN));

        nodelay (stdscr, FALSE);
        refresh ();
        napms (2000);
        getch ();
        game_deinit ();
    }
    else
    {
        attron (COLOR_PAIR (COLP_RED));
        switch (exitmsg)
        {
            case EXITMSG_COLLISION_OBSTACLE: mvprintw (LINES - 1, 1, "Game Over - you ran into an obstacle! Press any key to continue."); break;
            case EXITMSG_COLLISION_SELF:     mvprintw (LINES - 1, 1, "Game Over - you ran into yourself! Press any key to continue.");    break;
            case EXITMSG_OOB:                mvprintw (LINES - 1, 1, "Game Over - you went out of bounds! Press any key to continue.");   break;
            default: break;
        }
        attroff (COLOR_PAIR (COLP_RED));

        nodelay (stdscr, FALSE);
        refresh ();
        napms (500);
        getch ();
        game_deinit ();
    }
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
        mvprintw (food[i].pos.y + 1, (food[i].pos.x * 2) + 1, "");
        attroff (COLOR_PAIR (color));
    }
}



void
ui_draw_obstacles (pos_t obstacles[])
{
    attron (COLOR_PAIR (COLP_RED));
    for (int i = 0; i < (OBSTACLE_MAX_COUNT * OBSTACLE_MAX_LEN); i++)
        mvprintw (obstacles[i].y + 1, (obstacles[i].x * 2) + 1, "▒▒");
    attroff (COLOR_PAIR (COLP_RED));
}

