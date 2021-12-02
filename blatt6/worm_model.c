#include "worm_model.h"

#include "worm.h"
#include "board_model.h"



int worm_dx,     // heading of worm
    worm_dy,     // heading of worm
    worm_wcolor;

vec worm_pos[WORM_MAX_LEN]; // worm position in vector array, max length 20
int worm_head_index = 0;
int worm_len = 10;



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
    // delete last element
    place_item (
        // i have no idea why i need to subtract (not add) the worm length here, and nor do i have an idea why i need to
        // add the max worm length but it doesnt work without it so i might just leave it like that
        worm_pos[(worm_head_index - worm_len + WORM_MAX_LEN) % WORM_MAX_LEN].y,
        worm_pos[(worm_head_index - worm_len + WORM_MAX_LEN) % WORM_MAX_LEN].x,
        EMPTY_SYMBOL,
        COLP_EMPTY_CELL
    );
    // show new element
    place_item (
        worm_pos[worm_head_index].y,
        worm_pos[worm_head_index].x,
        WORM_SYMBOL,
        worm_wcolor
    );

    place_item (
        worm_pos[(worm_head_index + 19) % WORM_MAX_LEN].y,
        worm_pos[(worm_head_index + 19) % WORM_MAX_LEN].x,
        'o',
        worm_wcolor
    );


    place_item (
        worm_pos[(worm_head_index - worm_len + 21) % WORM_MAX_LEN].y,
        worm_pos[(worm_head_index - worm_len + 21) % WORM_MAX_LEN].x,
        '.',
        worm_wcolor
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

    for (int i = 1; i < worm_len; i++)
    {
        if (worm_pos[(worm_head_index - i + WORM_MAX_LEN) % WORM_MAX_LEN].x == worm_pos[worm_head_index].x &&
            worm_pos[(worm_head_index - i + WORM_MAX_LEN) % WORM_MAX_LEN].y == worm_pos[worm_head_index].y)
            *state = GAME_STATE_COLLISION;
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

