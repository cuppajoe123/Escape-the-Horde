#include "graphics.h"
#include "gold.h"


SDL_Rect **init_gold(SDL_Texture *gold_tex)
{
    SDL_Rect **gold_array = malloc(NUM_GOLD * sizeof(SDL_Rect *));
    if (!gold_array) {
        printf("Ran out of memory\n");
        exit(-1);
    }
    for (int i = 0; i < NUM_GOLD; i++) {
        gold_array[i] = malloc(sizeof(SDL_Rect));
        if (!gold_array[i]) {
            printf("Ran out of memory\n");
            exit(-1);
        }
        SDL_QueryTexture(gold_tex, NULL, NULL, &gold_array[i]->w, &gold_array[i]->h);
        gold_array[i]->w /= 10;
        gold_array[i]->h /= 10;
        gold_array[i]->x = rand() % WINDOW_WIDTH;
        gold_array[i]->y = rand() % WINDOW_HEIGHT;
    }

    return gold_array;
}

int check_gold_collisions(SDL_Rect player, SDL_Rect **gold_array, int score)
{
    for (int i = 0; i < NUM_GOLD; i++) {
        if (SDL_HasIntersection(&player, gold_array[i])) {
            score += 10;
            gold_array[i]->x = rand() % (WINDOW_WIDTH-30);
            gold_array[i]->y = rand() % (WINDOW_HEIGHT-30);
        }
    }
    return score;
}
