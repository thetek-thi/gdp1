#include <curses.h>

#include "color.h"
#include "game.h"



void
game_start ()
{
    erase ();
    refresh ();

    // draw game window outline
    game_draw_outline ();

    napms (10000);
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

