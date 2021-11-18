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
#define EMPTY_SYMBOL ' '

#define WORM_MAX_LEN 20



typedef enum _rescode
{
    RES_OK,
    RES_FAIL,
} rescode_t;

typedef enum _color_pair
{
    COLP_USER_WORM = 1,
    COLP_EMPTY_CELL,
} color_pair_t;

typedef enum _game_state
{
    GAME_STATE_ONGOING,
    GAME_STATE_OUT_OF_BOUNDS,
    GAME_STATE_QUIT,
} game_state_t;

typedef enum _worm_heading
{
    WORM_UP,
    WORM_DOWN,
    WORM_LEFT,
    WORM_RIGHT,
} worm_heading_t;

typedef struct _vec {
    int x;
    int y;
} vec;



int worm_dx,     // heading of worm
    worm_dy,     // heading of worm
    worm_wcolor;

vec worm_pos[WORM_MAX_LEN]; // worm position in vector array, max length 20
int worm_head_index = 0;
int worm_len = 5;



void      init_colors ();
void      read_user_input (game_state_t* state);
rescode_t do_level ();

void      app_init ();
void      app_deinit ();

void      place_item (int y, int x, chtype symbol, color_pair_t color);
int       get_last_row ();
int       get_last_col ();
rescode_t init_worm (int head_y, int head_x, worm_heading_t dir, color_pair_t color);
void      show_worm ();
void      move_worm (game_state_t* state);
void      set_worm_heading (worm_heading_t dir);



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
read_user_input (game_state_t* state)
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



rescode_t
do_level ()
{
    game_state_t state;
    rescode_t    res_code;
    int          bottom_left_y,
                 bottom_left_x;
    bool         end_level_loop;

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
place_item (int          y,
            int          x,
            chtype       symbol,
            color_pair_t color)
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
rescode_t
init_worm (int            head_y,
           int            head_x,
           worm_heading_t dir,
           color_pair_t   color)
{
    for (int i = 0; i < WORM_MAX_LEN; i++)
    {
        vec empty_pos = { -1, -1 };
        worm_pos[i] = empty_pos;
    }
    vec start_pos = { head_x, head_y };
    worm_pos[0] = start_pos;
    
    set_worm_heading (dir);

    worm_wcolor = color;

    return RES_OK;
}



// show the worms elements on display
void
show_worm ()
{
    // show new element
    place_item (
        worm_pos[worm_head_index].y,
        worm_pos[worm_head_index].x,
        WORM_SYMBOL,
        worm_wcolor
    );

    // delete last element
    place_item (
        // i have no idea why i need to subtract (not add) the worm length here, and nor do i have an idea why i need to
        // add the max worm length by a factor of two but it doesnt work without it so i might just leave it like that
        worm_pos[(worm_head_index - worm_len + 2*WORM_MAX_LEN) % WORM_MAX_LEN].y,
        worm_pos[(worm_head_index - worm_len + 2*WORM_MAX_LEN) % WORM_MAX_LEN].x,
        EMPTY_SYMBOL,
        COLP_EMPTY_CELL
    );
}



// move worm
void
move_worm (game_state_t* state)
{
    int worm_head_index_new = (worm_head_index + 1) % 20;
    worm_pos[worm_head_index_new] = worm_pos[worm_head_index];
    worm_head_index = worm_head_index_new;

    worm_pos[worm_head_index].x += worm_dx;
    worm_pos[worm_head_index].y += worm_dy;

    if (worm_pos[worm_head_index].x < 0 || worm_pos[worm_head_index].x > get_last_col () ||
        worm_pos[worm_head_index].y < 0 || worm_pos[worm_head_index].y > get_last_row ())
    {
        *state = GAME_STATE_OUT_OF_BOUNDS;
    }
}



// setters

void
set_worm_heading (worm_heading_t dir)
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

