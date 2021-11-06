#include <curses.h>
#include <stdbool.h>

#include "color.h"
#include "game.h"
#include "startscreen.h"



int selected_menu_opt;



void
start_screen_show ()
{
    int logo_pos = (COLS - 42) / 2;

    // worm logo
    attron (COLOR_PAIR (COLP_ACCENT));
    mvprintw (4, logo_pos, "██      ██    ████    ██████    ██      ██");
    mvprintw (5, logo_pos, "██      ██  ██    ██  ██    ██  ████  ████");
    mvprintw (6, logo_pos, "██  ██  ██  ██    ██  ██    ██  ██  ██  ██");
    mvprintw (7, logo_pos, "████  ████  ██    ██  ██████    ██      ██");
    mvprintw (8, logo_pos, "██      ██    ████    ██    ██  ██      ██");
    attroff (COLOR_PAIR (COLP_ACCENT));

    // draw menu options
    selected_menu_opt = 0;
    draw_menu_opts (12, logo_pos);

    // process user input
    nodelay (stdscr, FALSE);
    int end_loop = 0;
    while (end_loop == 0)
    {
        int ch = getch ();
        if (ch > 0)
        {
            switch (ch)
            {
                case 'w': case 'k': selected_menu_opt = (selected_menu_opt + 1) % 3; // fallthrough
                case 's': case 'j': selected_menu_opt = (selected_menu_opt + 1) % 3; draw_menu_opts (12, logo_pos); break;
                case 'q': end_loop = 1; break; // quit the game
                case ' ': end_loop = 2; break; // start the game - TODO: instead, execute the currently selected option
            }
        }
    }
    nodelay (stdscr, TRUE);

    if (end_loop == 2)
        game_start ();
}



void
draw_menu_opts (int line, int col)
{
    draw_menu_opt (line+0, col, "Play   ", selected_menu_opt == 0);
    draw_menu_opt (line+1, col, "Options", selected_menu_opt == 1);
    draw_menu_opt (line+2, col, "Quit   ", selected_menu_opt == 2);
}

void
draw_menu_opt (int line, int col, char* name, bool selected)
{
    if (selected)
    {
        attron (COLOR_PAIR (COLP_ACCENT));
        mvprintw (line, col, "» ");
    } else {
        mvprintw (line, col, "  ");
    }
    mvprintw (line, col+2, name);
    attroff (COLOR_PAIR (COLP_ACCENT));
}

