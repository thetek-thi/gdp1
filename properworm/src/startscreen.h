#include <stdbool.h>

#ifndef H_STARTSCREEN
#define H_STARTSCREEN

void start_screen_show ();

void draw_menu_opts (int line, int col);
void draw_menu_opt (int line, int col, char* name, bool selected);

#endif

