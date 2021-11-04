#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>



#define NAP_TIME 100
#define MIN_COLS 10
#define MIN_ROWS 3

#define WORM_SYMBOL '0'

#define WORM_STATUS_ONGOING 0
#define WORM_STATUS_OOB     1 // out of bounds
#define WORM_STATUS_QUIT    2



enum rescode
{
    RES_OK,
    RES_FAIL,
};

enum color_pair
{
    COLP_USER_WORM = 1,
};

enum game_state
{
    GAME_STATE_ONGOING,
    GAME_STATE_OUT_OF_BOUNDS,
    GAME_STATE_QUIT,
};

enum worm_heading
{
    WORM_UP,
    WORM_DOWN,
    WORM_LEFT,
    WORM_RIGHT,
};



int worm_head_y, // y coord of worm head
    worm_head_x, // x coord of worm head
    worm_dx,         // heading of worm
    worm_dy,         // heading of worm
    worm_wcolor;



void         init_colors ();
void         read_user_input (enum game_state* state);
enum rescode do_level ();

void         app_init ();
void         app_deinit ();

void         place_item (int y, int x, chtype symbol, enum color_pair color);
int          get_last_row ();
int          get_last_col ();
enum rescode init_worm (int head_y, int head_x, enum worm_heading dir, enum color_pair color);
void         show_worm ();
void         move_worm (enum game_state* state);
void         set_worm_heading (enum worm_heading dir);



// initialize colors of the game
void
init_colors (void)
{
    start_color ();
    init_pair (COLP_USER_WORM, COLOR_GREEN, COLOR_BLACK);
}



// read user input
void
read_user_input (enum game_state* state)
{
    int ch; // for storing the key codes

    if ((ch = getch ()) > 0)
    {
        switch (ch)
        {
            case 'q': *state = GAME_STATE_QUIT; break; // quit game
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



enum rescode
do_level ()
{
    enum game_state state;
    enum rescode    res_code;
    int             bottom_left_y,
                    bottom_left_x;
    bool            end_level_loop;

    state = GAME_STATE_ONGOING;

    bottom_left_y = get_last_row ();
    bottom_left_x = 0;

    res_code = init_worm (bottom_left_y, bottom_left_x, WORM_RIGHT, COLP_USER_WORM);
    if (res_code != RES_OK)
        return res_code;

    show_worm ();
    refresh ();

    end_level_loop = false;
    while (! end_level_loop)
    {
        read_user_input (&state);
        if (state == GAME_STATE_QUIT)
        {
            end_level_loop = true;
            continue;
        }

        move_worm (&state);

        if (state != GAME_STATE_ONGOING)
        {
            end_level_loop = true;
            continue;
        }

        show_worm ();
        napms (NAP_TIME);
        refresh ();
    }

    res_code = RES_OK;
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
place_item (int             y,
            int             x,
            chtype          symbol,
            enum color_pair color)
{
    move (y, x);
    attron (COLOR_PAIR (color));
    addch (symbol);
    attroff (COLOR_PAIR (color));
}



// getters

int get_last_row () { return LINES-1; }
int get_last_col () { return COLS-1;  }



// initialize the worm
enum rescode
init_worm (int               head_y,
           int               head_x,
           enum worm_heading dir,
           enum color_pair   color)
{
    worm_head_y = head_y;
    worm_head_x = head_x;
    
    set_worm_heading (dir);

    worm_wcolor = color;

    return RES_OK;
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
move_worm (enum game_state* state)
{
    worm_head_y += worm_dy;
    worm_head_x += worm_dx;

    if (worm_head_x < 0 || worm_head_x > get_last_col () ||
        worm_head_y < 0 || worm_head_y > get_last_row ())
    {
        *state = GAME_STATE_OUT_OF_BOUNDS;
    }
}



// setters

void
set_worm_heading (enum worm_heading dir)
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

