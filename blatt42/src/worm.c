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
           colorpair_t color)
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
}



void
worm_show (worm_t *worm)
{
    pos_t pos_head    = worm->pos[worm->headindex];
    pos_t pos_prehead = worm->pos[(worm->headindex + WORM_MAX_LEN - 1) % WORM_MAX_LEN];

    attron (COLOR_PAIR (worm->color));

    if (pos_head.x == pos_prehead.x - 1)
        mvprintw (pos_head.y + 1, pos_head.x * 2 + 1, "🭮█");
    else if (pos_head.x == pos_prehead.x + 1)
        mvprintw (pos_head.y + 1, pos_head.x * 2 + 1, "█🭬");
    else if (pos_head.y == pos_prehead.y - 1)
        mvprintw (pos_head.y + 1, pos_head.x * 2 + 1, "🭂🭍");
    else if (pos_head.y == pos_prehead.y + 1)
        mvprintw (pos_head.y + 1, pos_head.x * 2 + 1, "🭓🭞");

    mvprintw (pos_prehead.y + 1, pos_prehead.x * 2 + 1, "██");

    pos_t pos_tail    = worm->pos[(worm->headindex - worm->len + WORM_MAX_LEN + 1) % WORM_MAX_LEN];
    pos_t pos_pretail = worm->pos[(worm->headindex - worm->len + WORM_MAX_LEN + 2) % WORM_MAX_LEN];

    if (pos_tail.x == pos_pretail.x - 1)
        mvprintw (pos_tail.y + 1, pos_tail.x * 2 + 1, "🭨█");
    else if (pos_tail.x == pos_pretail.x + 1)
        mvprintw (pos_tail.y + 1, pos_tail.x * 2 + 1, "█🭪");
    else if (pos_tail.y == pos_pretail.y - 1)
        mvprintw (pos_tail.y + 1, pos_tail.x * 2 + 1, "🭍🭂");
    else if (pos_tail.y == pos_pretail.y + 1)
        mvprintw (pos_tail.y + 1, pos_tail.x * 2 + 1, "🭞🭓");
    else
        mvprintw (pos_tail.y + 1, pos_tail.x * 2 + 1, "██");

    attroff (COLOR_PAIR (worm->color));
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

        if (worm_check_collisions (worm))
        {
            ui_update_msg (worm, food, EXITMSG_COLLISION_SELF);
            return;
        }
        else if (worm_check_oob (worm))
        {
            ui_update_msg (worm, food, EXITMSG_OOB);
            return;
        }
        else if (food_count (food) <= 0)
        {
            ui_update_msg (worm, food, EXITMSG_SUCCESS);
            return;
        }
        else
            ui_update_msg (worm, food, EXITMSG_NONE);

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



int
food_count (food_t food[])
{
    int foodcount = 0;
    for (int i = 0; i < FOOD_COUNT; i++)
    {
        if (food[i].pos.x != -1 && food[i].pos.y != -1)
            foodcount++;
    }
    return foodcount;
}

