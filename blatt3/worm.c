#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>



#define TRUE 1
#define FALSE 0

#define NAP_TIME 100
#define MIN_COLS 10
#define MIN_ROWS 3

// numbers for color pairs used by curses macro COLOR_PAIR
#define COLP_USER_WORM 1

#define WORM_SYMBOL '0'

#define WORM_STATUS_ONGOING 0
#define WORM_STATUS_OOB     1 // out of bounds
#define WORM_STATUS_QUIT    2

#define WORM_UP    0
#define WORM_DOWN  1
#define WORM_LEFT  2
#define WORM_RIGHT 3




int worm_pos_head_y, // y coord of worm head
    worm_pos_head_x, // x coord of worm head
    worm_dx,         // heading of worm
    worm_dy,         // heading of worm
    worm_wcolor;



void init_colors ();
void read_user_input (int* game_state);
int  do_level ();

void app_init ();
void app_deinit ();

void place_item (int y, int x, chtype symbol, int color_pair);
int  get_last_row ();
int  get_last_col ();
int  init_worm (int head_y, int head_x, int dir, int color);
void show_worm ();
void move_worm (int* game_state);
void set_worm_heading (int dir);



// initialize colors of the game
void
init_colors (void)
{
    start_color ();
    init_pair (COLP_USER_WORM, COLOR_GREEN, COLOR_BLACK);
}



// read user input
void
read_user_input (int* game_start)
{
    int ch; // for storing the key codes

    if ((ch = getch ()) > 0)
    {
        switch (ch)
        {
            case 'q': *game_state = WORM_STATUS_QUIT; break; // quit game
            case KEY_UP:    set_worm_heading (WORM_UP);    break;
            case KEY_DOWN:  set_worm_heading (WORM_DOWN);  break;
            case KEY_LEFT:  set_worm_heading (WORM_LEFT);  break;
            case KEY_RIGHT: set_worm_heading (WORM_RIGHT); break;
            case 's': /* TODO: SINGLE STEP */ break; // single step
            case ' ': /* TODO: TERMINATE SIGNLE STEP */ break; // terminate signle step
        }
    }

    return;
}



int
do_level ()
{
    int game_state,
        res_code,
        end_level_loop,
        bottom_left_y,
        bottom_left_x;

    game_state = WORM_STATUS_ONGOING;

    bottom_left_y = get_last_row ();
    bottom_left_x = 0;

    res_code = init_worm (bottom_left_y, bottom_left_x, WORM_RIGHT, COLP_USER_WORM);
    if (res_code != EXIT_SUCCESS)
        return res_code;

    show_worm ();
    refresh ();

    end_level_loop = FALSE;
    while (end_level_loop == FALSE)
    {
        /* TODO: THIS STUFF RIGHT HERE */
    }

    res_code = EXIT_SUCCESS;
    return res_code;
}



/*
 * TODO: REST OF THE PROGRAM
 */

