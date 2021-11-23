#ifndef H_GAME
#define H_GAME

typedef enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
    DIR_NONE,
} direction;

void game_start ();
void game_draw_outline ();
void game_show_stats ();
void worm_tick ();
bool worm_pos_update (direction dir);
bool worm_check_oob ();

#endif

