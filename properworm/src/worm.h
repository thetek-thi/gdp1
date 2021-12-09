#include "color.h"
#include "defs.h"
#include "types.h"

#include <stdbool.h>



#ifndef _H_WORM
#define _H_WORM


typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
} dir_t;

typedef struct {
    pos_t       pos[WORM_MAX_LEN];
    int         len;
    int         len_pref;
    int         headindex;
    dir_t       dir;
    colorpair_t color;
} worm_t;


void worm_init             (worm_t *worm, pos_t startpos, int startlen, int startdir, colorpair_t color);
void worm_show             (worm_t *worm);
void worm_read_input       (worm_t *worm, food_t food[]);
void worm_move             (worm_t *worm);
bool worm_check_collisions (worm_t *worm);
bool worm_check_oob        (worm_t *worm);
void worm_check_food       (worm_t *worm, food_t food[]);
int  food_count            (food_t food[]);


#endif

