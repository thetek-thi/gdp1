#include "worm.h"

#include "color.h"
#include "defs.h"
#include "types.h"
#include "ui.h"

#include <curses.h>
#include <stdbool.h>



void
worm_init (worm_t     *worm,
           pos_t       startpos,
           int         startlen,
           int         startdir,
           colorpair_t color,
           colorpair_t color_dark,
           colorpair_t color_darker)
{
    pos_t invalid_pos = { -1, -1 };

    for (int i = 0; i < WORM_MAX_LEN; i++)
        worm->pos[i] = invalid_pos;

    worm->pos[0]       = startpos;
    worm->headindex    = 0;
    worm->len          = 3;
    worm->len_pref     = startlen;
    worm->dir          = startdir;
    worm->color        = color;
    worm->color_dark   = color_dark;
    worm->color_darker = color_darker;
}



void
worm_show (worm_t *worm)
{
    attron (COLOR_PAIR (worm->color));
    pos_t pos_head = worm->pos[worm->headindex];
    mvprintw (pos_head.y + 1, pos_head.x * 2 + 1, "██");
    attroff (COLOR_PAIR (worm->color));

    attron (COLOR_PAIR (worm->color_dark));
    pos_t pos_dark = worm->pos[(worm->headindex + WORM_MAX_LEN - 1) % WORM_MAX_LEN];
    mvprintw (pos_dark.y + 1, pos_dark.x * 2 + 1, "██");
    attroff (COLOR_PAIR (worm->color_dark));

    attron (COLOR_PAIR (worm->color_darker));
    pos_t pos_tail = worm->pos[(worm->headindex - worm->len + WORM_MAX_LEN + 1) % WORM_MAX_LEN];
    mvprintw (pos_tail.y + 1, pos_tail.x * 2 + 1, "██");
    attroff (COLOR_PAIR (worm->color_darker));
}



void
worm_read_input (worm_t *worm, food_t food[])
{
    int  input;
    bool endloop;

    while (! endloop)
    {
        input = getch ();
        switch (input)
        {
            case 'q': endloop = true; break;
            case 'w': worm->dir = DIR_UP;    break;
            case 'a': worm->dir = DIR_LEFT;  break;
            case 's': worm->dir = DIR_DOWN;  break;
            case 'd': worm->dir = DIR_RIGHT; break;
            case 'p': nodelay (stdscr, FALSE); break;
            case 'o': nodelay (stdscr, TRUE);  break;
        }

        worm_move       (worm);
        worm_show       (worm);
        worm_check_food (worm, food);
        ui_update_msg   (worm);

        if (worm_check_collisions (worm) || worm_check_oob (worm))
            return;

        napms (WORM_NAP_TIME);
    }
}



void
worm_move (worm_t *worm)
{
    pos_t newpos = worm->pos[worm->headindex];

    switch (worm->dir)
    {
        case DIR_UP:    newpos.y--; break;
        case DIR_DOWN:  newpos.y++; break;
        case DIR_LEFT:  newpos.x--; break;
        case DIR_RIGHT: newpos.x++; break;
    }

    pos_t clear_cell = worm->pos[(worm->headindex - worm->len + WORM_MAX_LEN + 1) % WORM_MAX_LEN];
    mvprintw (clear_cell.y + 1, clear_cell.x * 2 + 1, "  ");

    worm->headindex = (worm->headindex + 1) % WORM_MAX_LEN;
    worm->pos[worm->headindex] = newpos;

    if (worm->len < worm->len_pref)
        worm->len++;
}



bool
worm_check_collisions (worm_t *worm)
{
    for (int i = worm->headindex - 1; i > worm->headindex - worm->len; i--)
    {
        if (worm->pos[(i + WORM_MAX_LEN) % WORM_MAX_LEN].x == worm->pos[worm->headindex].x &&
            worm->pos[(i + WORM_MAX_LEN) % WORM_MAX_LEN].y == worm->pos[worm->headindex].y)
        {
            return true;
        }
    }

    return false;
}



bool
worm_check_oob (worm_t *worm)
{
    pos_t headpos = worm->pos[worm->headindex];
    return (headpos.x < 0 || headpos.x > (COLS / 2 - 2) || headpos.y < 0 || headpos.y > (LINES - 4));
}



void
worm_check_food (worm_t *worm, food_t food[])
{
    pos_t headpos = worm->pos[worm->headindex];

    for (int i = 0; i < FOOD_COUNT; i++)
    {
        if (food[i].pos.x == headpos.x && food[i].pos.y == headpos.y)
        {
            worm->len_pref += food[i].value;
            food[i].pos.x = -1;
            food[i].pos.y = -1;
        }
    }
}

