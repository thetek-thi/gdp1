#include "prep.h"

#include <curses.h>



// ncurses initializer
void
app_init ()
{
    initscr ();
    noecho (); cbreak (); nonl (); curs_set (0);
    keypad (stdscr, TRUE); nodelay (stdscr, TRUE);
}



// ncurses deinitializer
void
app_deinit ()
{
    standend ();
    refresh ();
    curs_set (1);
    endwin ();
}

