#ifndef _MESSAGES_H
#define _MESSAGES_H

#include "worm.h"
#include "worm_model.h"
#include "board_model.h"

void msgarea_clear_line  (int row);
void msgarea_show_border ();
void msgarea_show_status (worm* wormptr);
int  msgarea_show_dialog (char* prompt1, char* prompt2);

#endif

