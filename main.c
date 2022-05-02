/* TODO: change hero* to player* */


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
#include "gold.h"

struct enemy {
    SDL_Rect rect;
    int x_vel;
    int y_vel;
};

int main(void)
{
    init_graphics();

    SDL_Texture *player_tex = make_texture_img("resources/player.png");
    SDL_Rect player_rect;
    // prev_score_int is used to check if the score_int changed in a given frame
    long score_int = 0;
    long prev_score_int = 0;
    
    SDL_QueryTexture(player_tex, NULL, NULL, &player_rect.w, &player_rect.h);
    player_rect.w /= 10;
    player_rect.h /= 10;
    player_rect.x = WINDOW_WIDTH / 2;
    player_rect.y = WINDOW_HEIGHT / 2;
    int player_x_vel = 0;
    int player_y_vel = 0;

    bool move_up, move_down, move_right, move_left;
    move_up = move_down = move_right = move_left = false;
    
    bool close_requested = false;

    /* create scoreboard */
    SDL_Texture *score_tex = make_texture_str("0");
    SDL_Rect score_rect = make_textbox(score_tex, 0, 0, 1, 0x0);
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
        enemy_array[i]->rect.x = 0;
        enemy_array[i]->rect.y = 0;
        enemy_array[i]->x_vel = ENEMY_SPEED;
        enemy_array[i]->y_vel = ENEMY_SPEED;

    }

    SDL_Texture *gold_tex = make_texture_img("resources/gold.jpg");
    SDL_Rect **gold_array = init_gold(gold_tex);

    // start screen loop
    if (start_screen() == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(player_tex);
        SDL_DestroyTexture(gold_tex);
        SDL_DestroyTexture(score_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        for (int i = 0; i < NUM_ENEMIES; i++)
            free(enemy_array[i]);
        free(enemy_array);
        for (int i = 0; i < NUM_GOLD; i++)
            free(gold_array[i]);
        free(gold_array);
        TTF_Quit();
        return 0;
    }
    // start username_screen
    char username[15] = "";
    if (username_screen(username) == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(player_tex);
        SDL_DestroyTexture(gold_tex);
        SDL_DestroyTexture(score_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        for (int i = 0; i < NUM_ENEMIES; i++)
            free(enemy_array[i]);
        free(enemy_array);
        for (int i = 0; i < NUM_GOLD; i++)
            free(gold_array[i]);
        free(gold_array);
        TTF_Quit();
        return 0;
    }

    char score_str[256] = "0";

    while (!close_requested) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    TTF_CloseFont(font);
                    SDL_DestroyTexture(player_tex);
                    SDL_DestroyTexture(gold_tex);
                    SDL_DestroyTexture(score_tex);
                    SDL_DestroyRenderer(rend);
                    SDL_DestroyWindow(win);
                    for (int i = 0; i < NUM_ENEMIES; i++)
                        free(enemy_array[i]);
                    free(enemy_array);
                    for (int i = 0; i < NUM_GOLD; i++)
                        free(gold_array[i]);
                    free(gold_array);
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

        /* check player/wall collisions */
        if (player_rect.y <= 0) {
            player_rect.y = 0;
            player_y_vel = -player_y_vel;
        } else if (player_rect.y >= WINDOW_HEIGHT - player_rect.h) {
            player_rect.y = WINDOW_HEIGHT - player_rect.h;
            player_y_vel = -player_y_vel;
        }
        if (player_rect.x <= 0) {
            player_rect.x = 0;
            player_x_vel = -player_x_vel;
        } else if (player_rect.x >= WINDOW_WIDTH - player_rect.w) {
            player_rect.x = WINDOW_WIDTH - player_rect.w;
            player_x_vel = -player_x_vel;
        }

        /* change player velocity based on input */
        player_x_vel = player_y_vel = 0;
        if (move_up && !move_down) player_y_vel = -SPEED;
        else if (!move_up && move_down) player_y_vel = SPEED;

        if (move_right && !move_left) player_x_vel = SPEED;
        else if (!move_right && move_left) player_x_vel = -SPEED;

        player_rect.x += player_x_vel / 60;
        player_rect.y += player_y_vel / 60;

        SDL_RenderClear(rend);

        score_int = check_gold_collisions(player_rect, gold_array, prev_score_int);
        /* checks if score has changed */
        if (score_int > prev_score_int) {
            sprintf(score_str, "%ld", score_int);
            prev_score_int = score_int;
            score_tex = make_texture_str(score_str);
            score_rect = make_textbox(score_tex, 0, 0, 1, 0x0);
        }
        SDL_RenderCopy(rend, score_tex, NULL, &score_rect);

        for (int i = 0; i < NUM_GOLD; i++)
            SDL_RenderCopy(rend, gold_tex, NULL, gold_array[i]);

        /* Check where player is in relation to the enemy, and change enemy velocity accordingly */
        for (int i = 0; i < NUM_ENEMIES; i++) {
            if (enemy_array[i]->rect.x < player_rect.x)
                enemy_array[i]->x_vel = ENEMY_SPEED;
            else
                enemy_array[i]->x_vel = -ENEMY_SPEED;
            if (enemy_array[i]->rect.y < player_rect.y)
                enemy_array[i]->y_vel = ENEMY_SPEED;
            else
                enemy_array[i]->y_vel = -ENEMY_SPEED;

            /* check for collisions between enemies */
            for (int j = 0; j < NUM_ENEMIES; j++) {
                if (SDL_HasIntersection(&enemy_array[i]->rect, &enemy_array[j]->rect)) {
                    enemy_array[i]->rect.x -= enemy_array[i]->x_vel;
                    enemy_array[j]->rect.x += enemy_array[j]->x_vel;
                    enemy_array[i]->rect.y -= enemy_array[i]->y_vel;
                    enemy_array[j]->rect.y += enemy_array[j]->y_vel;
                }
            }

            /* check for enemy collision with player */
            if (SDL_HasIntersection(&enemy_array[i]->rect, &player_rect))
                goto end_game;
            enemy_array[i]->rect.x += enemy_array[i]->x_vel;
            enemy_array[i]->rect.y += enemy_array[i]->y_vel;

            SDL_RenderCopy(rend, enemy_tex, NULL, &enemy_array[i]->rect);
        }

        SDL_RenderCopy(rend, player_tex, NULL, &player_rect);
        SDL_RenderPresent(rend);

        SDL_Delay(1000/60);
    }


end_game:

    save_user_data(score_str, username);

    leader_board_screen();
    
    // clean up resources before exiting
    TTF_CloseFont(font);
    SDL_DestroyTexture(player_tex);
    SDL_DestroyTexture(gold_tex);
    SDL_DestroyTexture(score_tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    for (int i = 0; i < NUM_ENEMIES; i++)
        free(enemy_array[i]);
    free(enemy_array);
    for (int i = 0; i < NUM_GOLD; i++)
        free(gold_array[i]);
    free(gold_array);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
