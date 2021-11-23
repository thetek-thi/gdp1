#include <curses.h>

#include "color.h"
#include "game.h"



int worm_length  = 0,
    worm_time_ms = 0;

int worm_pos_x = 1,
    worm_pos_y = 1;
direction worm_dir = DIR_RIGHT;



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
        int ch = getch ();
        direction dir = DIR_NONE;
        switch (ch)
        {
            case 'q': end_loop = 1; break;
            case 'w': case 'k': dir = DIR_UP;    break;
            case 'a': case 'h': dir = DIR_LEFT;  break;
            case 's': case 'j': dir = DIR_DOWN;  break;
            case 'd': case 'l': dir = DIR_RIGHT; break;
            default: worm_tick (); dir = DIR_NONE;
        }
        if (ch != 'q')
            worm_tick ();
        if (worm_pos_update (dir))
            end_loop = 1;
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



void
worm_tick ()
{
    napms (100);
    worm_time_ms += 100;
    game_show_stats ();
}



bool
worm_pos_update (direction dir)
{
    if (dir != DIR_NONE)
        worm_dir = dir;

    switch (worm_dir)
    {
        case DIR_LEFT:  worm_pos_x--; break;
        case DIR_RIGHT: worm_pos_x++; break;
        case DIR_UP:    worm_pos_y--; break;
        case DIR_DOWN:  worm_pos_y++; break;
        default: break;
    }

    bool oob = worm_check_oob ();
    if (oob) return true;

    attron (COLOR_PAIR (COLP_ACCENT));
    mvprintw (worm_pos_y+1, worm_pos_x*2+1, "██");
    attroff (COLOR_PAIR (COLP_ACCENT));

    return false;
}



bool
worm_check_oob ()
{
    if (worm_pos_x <= 0 || worm_pos_x > (COLS/2)-3 ||
        worm_pos_y <  0 || worm_pos_y > LINES-3)
        return true;
    return false;
}

