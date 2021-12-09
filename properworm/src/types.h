#ifndef _H_TYPES
#define _H_TYPES


typedef struct {
    int x;
    int y;
} pos_t;


typedef enum {
    DIR_DOWN,
    DIR_RIGHT,
    DIR_LEFT,
    DIR_UP,
} dir_t;


typedef struct {
    pos_t pos;
    int   value;
} food_t;


typedef enum {
    EXITMSG_NONE,
    EXITMSG_SUCCESS,
    EXITMSG_COLLISION_OBSTACLE,
    EXITMSG_COLLISION_SELF,
    EXITMSG_OOB,
} exitmsg_t;


#endif

