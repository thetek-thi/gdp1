#include "messages.h"

#include "worm.h"
#include "board_model.h"
#include "worm_model.h"

#include <curses.h>



void
msgarea_clear_line (int row)
{
    move (row, 0);

    for (int i = 1; i <= COLS; i++)
        addch (' ');
}



void
msgarea_show_border ()
{
    int pos_line0 = LINES - ROWS_RESERVED ;

    for (int i = 0; i < COLS ; i++) {
        move (pos_line0, i);
        attron (COLOR_PAIR (COLP_BARRIER));
        addch (SYMBOL_BARRIER);
        attroff (COLOR_PAIR (COLP_BARRIER));
    }
}



void
msgarea_show_status (worm* wormptr)
{
    int pos_line2 = LINES - ROWS_RESERVED + 2;

    vec headpos = worm_get_head (wormptr);
    mvprintw (pos_line2, 1, "Wurm ist an Position: y=%3d x=%3d", headpos.y, headpos.x);
}



int
msgare_show_dialog (char* prompt1, char* prompt2)
{
    int pos_line1 = LINES - ROWS_RESERVED + 1;
    int pos_line2 = LINES - ROWS_RESERVED + 2;
    int pos_line3 = LINES - ROWS_RESERVED + 3;

    if (prompt1 == NULL)
        return RES_FAILED;

    // delete lines in the message area
    msgarea_clear_line (pos_line1);
    msgarea_clear_line (pos_line2);
    msgarea_clear_line (pos_line3);

    // display message
    mvprintw (pos_line2, 1, "%s", prompt1);
    if (prompt2 != NULL)
        mvprintw (pos_line3, 1, "%s", prompt2);
    refresh ();

    nodelay (stdscr, FALSE);
    int ch = getch ();   // wait for user to press an arbitrary key
    nodelay (stdscr, TRUE);

    // delete lines in the message area
    msgarea_clear_line (pos_line1);
    msgarea_clear_line (pos_line2);
    msgarea_clear_line (pos_line3);

    // display changes
    refresh ();

    // return code of key pressed
    return ch; 
}

