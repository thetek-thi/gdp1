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




int worm_head_y, // y coord of worm head
    worm_head_x, // x coord of worm head
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
read_user_input (int* game_state)
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
            case 's': nodelay (stdscr, FALSE); break; // single step
            case ' ': nodelay (stdscr, TRUE);  break; // terminate signle step
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
        read_user_input (&game_state);
        if (game_state == WORM_STATUS_QUIT)
        {
            end_level_loop = TRUE;
            continue;
        }

        move_worm (&game_state);

        if (game_state != WORM_STATUS_ONGOING)
        {
            end_level_loop = TRUE;
            continue;
        }

        show_worm ();
        napms (NAP_TIME);
        refresh ();
    }

    res_code = EXIT_SUCCESS;
    return res_code;
}



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



// place item on board
void
place_item (int    y,
            int    x,
            chtype symbol,
            int    color_pair)
{
    move (y, x);
    attron (COLOR_PAIR (color_pair));
    addch (symbol);
    attroff (COLOR_PAIR (color_pair));
}



// getters

int get_last_row () { return LINES-1; }
int get_last_col () { return COLS-1;  }



// initialize the worm
int
init_worm (int head_y,
           int head_x,
           int dir,
           int color)
{
    worm_head_y = head_y;
    worm_head_x = head_x;
    
    set_worm_heading (dir);

    worm_wcolor = color;

    return EXIT_SUCCESS;
}



// show the worms elements on display
void
show_worm ()
{
    place_item (
        worm_head_y,
        worm_head_x,
        WORM_SYMBOL,
        worm_wcolor
    );
}



// move worm
void
move_worm (int* game_state)
{
    worm_head_y += worm_dy;
    worm_head_x += worm_dx;

    if (worm_head_x < 0 || worm_head_x > get_last_col () ||
        worm_head_y < 0 || worm_head_y > get_last_row ())
    {
        *game_state = WORM_STATUS_OOB;
    }
}



// setters

void
set_worm_heading (int dir)
{
    switch (dir)
    {
        case WORM_UP:    worm_dx =  0; worm_dy = -1; break;
        case WORM_DOWN:  worm_dx =  0; worm_dy =  1; break;
        case WORM_LEFT:  worm_dx = -1; worm_dy =  0; break;
        case WORM_RIGHT: worm_dx =  1; worm_dy =  0; break;
    }
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

