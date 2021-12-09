#include "game.h"

#include "color.h"
#include "types.h"
#include "ui.h"
#include "worm.h"

#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>



void
game_init ()
{
    setlocale (LC_ALL, "");

    initscr ();

    noecho   ();
    cbreak   ();
    nonl     ();
    curs_set (0);
    keypad   (stdscr, TRUE);
    nodelay  (stdscr, TRUE);

    attron (COLOR_PAIR (COLP_FG));

    time_t t;
    srand((unsigned) time(&t));
}



void
game_deinit ()
{
    standend ();
    refresh  ();
    curs_set (1);
    endwin   ();
}



void
game_run ()
{
    ui_draw_frame ();

    worm_t worm;
    pos_t  startpos = { 1, 1 };
    worm_init (&worm, startpos, 10, DIR_RIGHT, COLP_GREEN);

    food_t food[FOOD_COUNT];
    for (int i = 0; i < FOOD_COUNT; i++)
    {
        pos_t  foodpos  = { rand () % (COLS / 2 - 2), rand () % (LINES - 4) };
        food_t fooditem = { foodpos, (rand () % 3 + 1) * 2 };
        food[i] = fooditem;
    }

    ui_draw_food (food);

    pos_t obstacles[OBSTACLE_MAX_COUNT * OBSTACLE_MAX_LEN];
    int   index = 0;
    for (int i = 0; i < OBSTACLE_MIN_COUNT + rand () % (OBSTACLE_MAX_COUNT - OBSTACLE_MIN_COUNT); i++)
    {
        pos_t obstacle_pos        = { rand () % (COLS / 2 - 2 - OBSTACLE_MAX_LEN), rand () % (LINES - 4 - OBSTACLE_MAX_LEN) };
        int   obstacle_extend_len = OBSTACLE_MIN_LEN + rand () % (OBSTACLE_MAX_LEN - OBSTACLE_MIN_LEN);
        dir_t obstacle_extend_dir = rand () % 2;
        for (int j = 0; j < obstacle_extend_len; j++)
        {
            obstacles[index] = obstacle_pos;
            switch (obstacle_extend_dir)
            {
                case DIR_DOWN:  obstacle_pos.y++; break;
                case DIR_RIGHT: obstacle_pos.x++; break;
                default: break;
            }
            index++;
        }
    }
    for (; index < OBSTACLE_MAX_COUNT * OBSTACLE_MAX_LEN; index++)
    {
        pos_t empty_obstacle = { -1, -1 };
        obstacles[index] = empty_obstacle;
    }

    ui_draw_obstacles (obstacles);

    worm_show (&worm);
    worm_read_input (&worm, food, obstacles);
}

