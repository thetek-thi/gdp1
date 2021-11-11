#include <curses.h>
#include <stdlib.h>
#include <string.h>



void
app_init (void)
{
    initscr (); noecho (); cbreak (); nonl (); curs_set (0);
    keypad (stdscr, TRUE); nodelay (stdscr, TRUE);
}



void
app_deinit (void)
{
    standend (); refresh (); curs_set (1);
    endwin ();
}



int
main (void)
{
    int rescode;
    char* msg_tmpl = "window size: XXX rows, YYY cols";
    int msg_len;
    int min_rows = 3;

    app_init ();

    msg_len = strlen (msg_tmpl);

    if (LINES < min_rows || COLS < msg_len) {
        app_deinit ();
        printf ("window to small: we need %dx%d", msg_len, min_rows);
        rescode = EXIT_FAILURE;
    } else {
        int mid_row = LINES / 2;
        int start_col = (COLS - msg_len) / 2;

        move (0,       0     ); addch ('A');
        move (0,       COLS-1); addch ('B');
        move (LINES-1, 0     ); addch ('C');
        move (LINES-1, COLS-1); addch ('D');
        
        mvprintw (mid_row, start_col, "window size: %3d rows, %3d cols", LINES, COLS);

        refresh ();

        nodelay (stdscr, FALSE);
        getch ();

        app_deinit ();
        rescode = EXIT_SUCCESS;
    }

    return rescode;
}

