#include "game.h"

#include "color.h"
#include "types.h"
#include "ui.h"
#include "worm.h"

#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>



void
game_init ()
{
    setlocale (LC_ALL, "");

    initscr ();

    noecho   ();
    cbreak   ();
    nonl     ();
    curs_set (0);
    keypad   (stdscr, TRUE);
    nodelay  (stdscr, TRUE);

    attron (COLOR_PAIR (COLP_FG));

    time_t t;
    srand((unsigned) time(&t));
}



void
game_deinit ()
{
    standend ();
    refresh  ();
    curs_set (1);
    endwin   ();
}



void
game_run ()
{
    ui_draw_frame ();

    worm_t worm;
    pos_t  startpos = { 1, 1 };
    worm_init (&worm, startpos, 10, DIR_RIGHT, COLP_GREEN, COLP_DARKGREEN, COLP_DARKERGREEN);

    food_t food[FOOD_COUNT];
    for (int i = 0; i < FOOD_COUNT; i++)
    {
        pos_t  foodpos  = { rand () % (COLS / 2 - 2), rand () % (LINES - 4) };
        food_t fooditem = { foodpos, (rand () % 3 + 1) * 2 };
        food[i] = fooditem;
    }

    ui_draw_food (food);

    worm_show (&worm);
    worm_read_input (&worm, food);
}

