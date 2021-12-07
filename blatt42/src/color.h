#ifndef _H_COLOR
#define _H_COLOR


typedef enum {
    COLP_BLUE = 1,
    COLP_FG,
    COLP_GREY,
    COLP_GREEN,
    COLP_ORANGE,
    COLP_PURPLE,
    COLP_VIOLET,
} colorpair_t;

typedef enum {
    COL_BG,
    COL_BLUE,
    COL_FG,
    COL_GREY,
    COL_GREEN,
    COL_ORANGE,
    COL_PURPLE,
    COL_VIOLET,
} color_t;


void colors_init ();


#endif

