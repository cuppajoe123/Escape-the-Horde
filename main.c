/*
 * This program was written using the SDL2 graphics library. All functions and data types beginning in SDL* are from the graphics library:
 * https://github.com/libsdl-org/SDL
 *
 * The font used for the user interface is Fira Code: https://github.com/tonsky/FiraCode
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "graphics.h"
#include "leader_board.h"

struct enemy {
    SDL_Rect rect;
    int x_vel;
    int y_vel;
};

int main(void)
{
    init_graphics();

    SDL_Texture *hero_tex = make_texture_img("resources/hero.png");
    SDL_Rect hero_rect;
    SDL_QueryTexture(hero_tex, NULL, NULL, &hero_rect.w, &hero_rect.h);
    hero_rect.w /= 10;
    hero_rect.h /= 10;
    hero_rect.x = WINDOW_WIDTH / 2;
    hero_rect.y = WINDOW_HEIGHT / 2;
    int hero_x_vel = 0;
    int hero_y_vel = 0;

    bool move_up, move_down, move_right, move_left;
    move_up = move_down = move_right = move_left = false;
    
    bool close_requested = false;

    /* create enemies */
    SDL_Texture *enemy_tex = make_texture_img("resources/enemy.jpg");
    struct enemy **enemy_array = malloc(NUM_ENEMIES * sizeof(struct enemy *));
    if (!enemy_array) {
        printf("Ran out of memory\n");
        exit(-1);
    }
    srand(time(0));
    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemy_array[i] = malloc(sizeof(struct enemy));
        if (!enemy_array[i]) {
            printf("Ran out of memory\n");
            exit(-1);
        }
        SDL_QueryTexture(enemy_tex, NULL, NULL, &enemy_array[i]->rect.w, &enemy_array[i]->rect.h);
        enemy_array[i]->rect.w /= 18;
        enemy_array[i]->rect.h /= 18;
        enemy_array[i]->rect.x = rand() % WINDOW_WIDTH;
        enemy_array[i]->rect.y = rand() % WINDOW_HEIGHT;
        enemy_array[i]->x_vel = SPEED / 100;
        enemy_array[i]->y_vel = SPEED / 100;

    }

    // start screen loop
    if (start_screen() == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(hero_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        for (int i = 0; i < NUM_ENEMIES; i++)
            free(enemy_array[i]);
        free(enemy_array);
        TTF_Quit();
        return 0;
    }
    // start username_screen
    char username[15] = "";
    if (username_screen(username) == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(hero_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        for (int i = 0; i < NUM_ENEMIES; i++)
            free(enemy_array[i]);
        free(enemy_array);
        TTF_Quit();
        return 0;
    }
    while (!close_requested) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    TTF_CloseFont(font);
                    SDL_DestroyTexture(hero_tex);
                    SDL_DestroyRenderer(rend);
                    SDL_DestroyWindow(win);
                    for (int i = 0; i < NUM_ENEMIES; i++)
                        free(enemy_array[i]);
                    free(enemy_array);
                    TTF_Quit();
                    SDL_Quit();
                    return 0;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            move_up = true;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            move_down = true;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            move_right = true;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            move_left = true;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            move_up = false;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            move_down = false;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            move_right = false;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            move_left = false;
                            break;
                    }
                    break;
            }
        }

        if (hero_rect.y <= 0) {
            hero_rect.y = 0;
            hero_y_vel = -hero_y_vel;
        } else if (hero_rect.y >= WINDOW_HEIGHT - hero_rect.h) {
            hero_rect.y = WINDOW_HEIGHT - hero_rect.h;
            hero_y_vel = -hero_y_vel;
        }
        if (hero_rect.x <= 0) {
            hero_rect.x = 0;
            hero_x_vel = -hero_x_vel;
        } else if (hero_rect.x >= WINDOW_WIDTH - hero_rect.w) {
            hero_rect.x = WINDOW_WIDTH - hero_rect.w;
            hero_x_vel = -hero_x_vel;
        }

        hero_x_vel = hero_y_vel = 0;
        if (move_up && !move_down) hero_y_vel = -SPEED;
        else if (!move_up && move_down) hero_y_vel = SPEED;

        if (move_right && !move_left) hero_x_vel = SPEED;
        else if (!move_right && move_left) hero_x_vel = -SPEED;

        hero_rect.x += hero_x_vel / 60;
        hero_rect.y += hero_y_vel / 60;

        SDL_RenderClear(rend);
        /* Check where player is in relation to the enemy, and change enemy velocity accordingly */
        for (int i = 0; i < NUM_ENEMIES; i++) {
            if (enemy_array[i]->rect.x < hero_rect.x)
                enemy_array[i]->x_vel = SPEED / 100;
            else
                enemy_array[i]->x_vel = -SPEED / 100;
            if (enemy_array[i]->rect.y < hero_rect.y)
                enemy_array[i]->y_vel = SPEED / 100;
            else
                enemy_array[i]->y_vel = -SPEED / 100;

            /*
            for (int j = 0; j < NUM_ENEMIES; j++) {
                if ((enemy_array[i]->rect.x >= enemy_array[j]->rect.x - enemy_array[j]->rect.w/2)
                    && (enemy_array[i]->rect.x <= enemy_array[j]->rect.x + enemy_array[j]->rect.w/2))
                    enemy_array[i]->x_vel *= -1;
                if ((enemy_array[i]->rect.y >= enemy_array[j]->rect.y - enemy_array[j]->rect.h/2)
                    && (enemy_array[i]->rect.y <= enemy_array[j]->rect.y + enemy_array[j]->rect.h/2))
                    enemy_array[i]->y_vel *= -1;
            }
            */

            /*
            for (int j = 0; j < NUM_ENEMIES; j++) {
                if (SDL_HasIntersection(&enemy_array[i]->rect, &enemy_array[j]->rect)) {
                    enemy_array[i]->x_vel *= -1;
                    enemy_array[i]->y_vel *= -1;
                }
            }
            */
            /* check for collision with player */
            if (SDL_HasIntersection(&enemy_array[i]->rect, &hero_rect)) {
                enemy_array[i]->x_vel *= -1;
                enemy_array[i]->y_vel *= -1;
            }
            enemy_array[i]->rect.x += enemy_array[i]->x_vel;
            enemy_array[i]->rect.y += enemy_array[i]->y_vel;

            SDL_RenderCopy(rend, enemy_tex, NULL, &enemy_array[i]->rect);
        }

        SDL_RenderCopy(rend, hero_tex, NULL, &hero_rect);
        SDL_RenderPresent(rend);

        SDL_Delay(1000/60);
    }

    if (leader_board_screen() == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(hero_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        for (int i = 0; i < NUM_ENEMIES; i++)
            free(enemy_array[i]);
        free(enemy_array);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // clean up resources before exiting
    TTF_CloseFont(font);
    SDL_DestroyTexture(hero_tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    for (int i = 0; i < NUM_ENEMIES; i++)
        free(enemy_array[i]);
    free(enemy_array);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
