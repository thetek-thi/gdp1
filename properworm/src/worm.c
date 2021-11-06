#include <curses.h>
#include <stdlib.h>
#include <stdio.h>

#include "screen.h"
#include "startscreen.h"



int
main (void)
{
    // init screen
    screen_init ();

    // check if terminal is big enough
    if (LINES < 20 || COLS < 60)
    {
        printf ("Terminal too small! At least 20x60 is required.");
        return EXIT_FAILURE;
    }

    // show start screen
    start_screen_show ();

    // deinit screen
    screen_deinit ();

    return EXIT_SUCCESS;
}

