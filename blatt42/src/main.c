#include "color.h"
#include "defs.h"
#include "game.h"

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>



int
main ()
{
    game_init ();
    colors_init ();

    if (LINES < MIN_LINES || COLS < MIN_COLS)
    {
        game_deinit ();
        printf ("Window too small! We need at least %dx%d\n", MIN_COLS, MIN_LINES);
        return EXIT_FAILURE;
    }

    game_run ();

    game_deinit ();

    return EXIT_SUCCESS;
}

