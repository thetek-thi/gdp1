#include <curses.h>

#include "color.h"
#include "game.h"



int worm_length  = 0,
    worm_time_ms = 0;



void
game_start ()
{
    erase ();
    refresh ();

    // draw game window outline
    game_draw_outline ();

    // add game stats
    game_show_stats ();

    int end_loop = 0;
    while (end_loop == 0)
    {
        if (getch () == 'q')
        {
            end_loop = 1;
        } else {
            napms (100);
            worm_time_ms += 100;
            game_show_stats ();
        }
    }
}



void
game_draw_outline ()
{
    attron (COLOR_PAIR (COLP_GREY));

    move (0, 1);
    printw ("┌");
    
    for (int i = 2; i < COLS-2; i++)
    {
        move (0, i);
        printw ("─");
    }

    move (0, COLS-2);
    printw ("┐");

    for (int i = 0; i < LINES-3; i++)
    {
        move (i+1, 1);      printw ("│");
        move (i+1, COLS-2); printw ("│");
    }

    move (LINES-2, 1);
    printw ("└");
    
    for (int i = 2; i < COLS-2; i++)
    {
        move (LINES-2, i);
        printw ("─");
    }

    move (LINES-2, COLS-2);
    printw ("┘");

    attroff (COLOR_PAIR (COLP_GREY));
    refresh ();
}



void
game_show_stats ()
{
    attron (COLOR_PAIR (COLP_ACCENT));
    attron (A_BOLD);
    mvprintw (LINES-1, 3, "Length:");
    attroff (COLOR_PAIR (COLP_ACCENT));
    attroff (A_BOLD);
    mvprintw (LINES-1, 11, "%3d", worm_length);

    attron (COLOR_PAIR (COLP_ACCENT));
    attron (A_BOLD);
    mvprintw (LINES-1, 18, "Time:");
    attroff (COLOR_PAIR (COLP_ACCENT));
    attroff (A_BOLD);
    mvprintw (LINES-1, 24, "%6.1fs", worm_time_ms / 1000.0);

    refresh ();
}

