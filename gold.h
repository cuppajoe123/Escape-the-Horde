#ifndef GOLD_H
#define GOLD_H

#include "graphics.h"

#define NUM_GOLD 5

/* Gold pieces are the objectives. There are 5 gold pieces on the screen at any one time */

SDL_Rect **init_gold(SDL_Texture *gold_tex);
void check_gold_collisions(SDL_Rect player_rect, SDL_Rect **gold_array);

#endif
