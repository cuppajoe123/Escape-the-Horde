/*
 * This program was written using the SDL2 graphics library. All functions and data types beginning in SDL* are from the graphics library:
 * https://github.com/libsdl-org/SDL
 *
 * The font used for the user interface is Fira Code: https://github.com/tonsky/FiraCode
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphics.h"
#include "leader_board.h"

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
    int hero_x_vel = SPEED;
    int hero_y_vel = SPEED;

    bool move_up, move_down, move_right, move_left;
    move_up = move_down = move_right = move_left = false;
    
    bool close_requested = false;

    // start screen loop
    if (start_screen() == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(hero_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
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

        SDL_RenderCopy(rend, hero_tex, NULL, &hero_rect);
        SDL_RenderPresent(rend);

        SDL_Delay(1000/60);
    }


    if (leader_board_screen() == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(hero_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // clean up resources before exiting
    TTF_CloseFont(font);
    SDL_DestroyTexture(hero_tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}






