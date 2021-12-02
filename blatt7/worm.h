#include "worm_model.h"



#ifndef _WORM_H
#define _WORM_H


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
    GAME_STATE_COLLISION,
    GAME_STATE_QUIT,
} game_state_t;

typedef enum _worm_heading
{
    WORM_UP,
    WORM_DOWN,
    WORM_LEFT,
    WORM_RIGHT,
} worm_heading_t;


void      init_colors     ();
void      read_user_input (worm_t* worm, game_state_t* state);
rescode_t do_level        ();


#endif

