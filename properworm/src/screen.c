#include <curses.h>
#include <locale.h>

#include "color.h"



/* initialize ncurses */

void
screen_init ()
{
    setlocale (LC_ALL, "");

    initscr ();

    noecho ();
    cbreak ();
    nonl ();
    curs_set (0);
    
    keypad (stdscr, TRUE);
    nodelay (stdscr, TRUE);

    start_color ();
    setup_color_pairs ();
}



/* deinizialize ncurses */

void
screen_deinit ()
{
    standend ();
    refresh ();
    curs_set (1);
    endwin ();
}

