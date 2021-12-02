#include "worm.h"

#include "prep.h"
#include "worm_model.h"
#include "board_model.h"

#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>



// initialize colors of the game
void
init_colors (void)
{
    start_color ();
    init_pair (COLP_USER_WORM,  COLOR_GREEN, COLOR_BLACK);
    init_pair (COLP_EMPTY_CELL, COLOR_WHITE, COLOR_BLACK);
}



// read user input
void
read_user_input (worm_t*       worm,
                 game_state_t* state)
{
    int ch; // for storing the key codes

    if ((ch = getch ()) > 0)
    {
        switch (ch)
        {
            case 'q': *state = GAME_STATE_QUIT; break; // quit game
            case KEY_UP:    set_worm_heading (worm, WORM_UP);    break;
            case KEY_DOWN:  set_worm_heading (worm, WORM_DOWN);  break;
            case KEY_LEFT:  set_worm_heading (worm, WORM_LEFT);  break;
            case KEY_RIGHT: set_worm_heading (worm, WORM_RIGHT); break;
            case 's': nodelay (stdscr, FALSE); break; // single step
            case ' ': nodelay (stdscr, TRUE);  break; // terminate signle step
        }
    }

    return;
}



rescode_t
do_level ()
{
    game_state_t state;
    rescode_t    res_code;
    int          bottom_left_y,
                 bottom_left_x;
    bool         end_level_loop;
    worm_t       worm;

    state = GAME_STATE_ONGOING;

    bottom_left_y = get_last_row ();
    bottom_left_x = 0;

    res_code = init_worm (&worm, 15, bottom_left_y, bottom_left_x, WORM_RIGHT, COLP_USER_WORM);
    if (res_code != RES_OK)
        return res_code;

    show_worm (&worm);
    refresh ();

    end_level_loop = false;
    while (! end_level_loop)
    {
        read_user_input (&worm, &state);
        if (state == GAME_STATE_QUIT)
        {
            end_level_loop = true;
            continue;
        }

        move_worm (&worm, &state);

        if (state != GAME_STATE_ONGOING)
        {
            end_level_loop = true;
            continue;
        }

        show_worm (&worm);
        napms (NAP_TIME);
        refresh ();
    }

    res_code = RES_OK;
    return res_code;
}



int
main (void)
{
    int res_code;

    app_init ();

    init_colors ();

    if (LINES < MIN_ROWS || COLS < MIN_COLS)
    {
        app_deinit ();
        printf ("window too small - we need at least %dx%d\n", MIN_COLS, MIN_ROWS);
        res_code = EXIT_FAILURE;
    } else {
        res_code = do_level ();
        app_deinit ();
    }

    return res_code;
}

