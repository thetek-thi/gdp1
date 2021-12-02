#include "worm_model.h"

#include "worm.h"
#include "board_model.h"



// initialize the worm
rescode_t
init_worm (worm_t*        worm,
           int            head_y,
           int            head_x,
           int            len,
           worm_heading_t dir,
           color_pair_t   color)
{
    for (int i = 0; i < WORM_MAX_LEN; i++)
    {
        vec empty_pos = { -1, -1 };
        worm->pos[i] = empty_pos;
    }
    vec start_pos = { head_x, head_y };
    worm->pos[0] = start_pos;

    worm->head_index = 0;
    worm->len = len;
    
    set_worm_heading (worm, dir);

    worm->color = color;

    return RES_OK;
}



// show the worms elements on display
void
show_worm (worm_t* worm)
{
    // show new element
    place_item (
        worm->pos[worm->head_index].y,
        worm->pos[worm->head_index].x,
        WORM_SYMBOL,
        worm->color
    );

    // delete last element
    place_item (
        // i have no idea why i need to subtract (not add) the worm length here, and nor do i have an idea why i need to
        // add the max worm length but it doesnt work without it so i might just leave it like that
        worm->pos[(worm->head_index - worm->len + WORM_MAX_LEN) % WORM_MAX_LEN].y,
        worm->pos[(worm->head_index - worm->len + WORM_MAX_LEN) % WORM_MAX_LEN].x,
        EMPTY_SYMBOL,
        COLP_EMPTY_CELL
    );
}



// move worm
void
move_worm (worm_t*       worm,
           game_state_t* state)
{
    int worm_head_index_new = (worm->head_index + 1) % 20;
    worm->pos[worm_head_index_new] = worm->pos[worm->head_index];
    worm->head_index = worm_head_index_new;

    worm->pos[worm->head_index].x += worm->dx;
    worm->pos[worm->head_index].y += worm->dy;

    if (worm->pos[worm->head_index].x < 0 || worm->pos[worm->head_index].x > get_last_col () ||
        worm->pos[worm->head_index].y < 0 || worm->pos[worm->head_index].y > get_last_row ())
    {
        *state = GAME_STATE_OUT_OF_BOUNDS;
    }

    for (int i = 1; i < worm_len; i++)
    {
        if (worm->pos[(worm->head_index - i + WORM_MAX_LEN) % WORM_MAX_LEN].x == worm->pos[worm->head_index].x &&
            worm->pos[(worm->head_index - i + WORM_MAX_LEN) % WORM_MAX_LEN].y == worm->pos[worm->head_index].y)
            *state = GAME_STATE_COLLISION;
    }
}




// setters

void
set_worm_heading (worm_t*        worm,
                  worm_heading_t dir)
{
    switch (dir)
    {
        case WORM_UP:    worm->dx =  0; worm->dy = -1; break;
        case WORM_DOWN:  worm->dx =  0; worm->dy =  1; break;
        case WORM_LEFT:  worm->dx = -1; worm->dy =  0; break;
        case WORM_RIGHT: worm->dx =  1; worm->dy =  0; break;
    }
}

