#include "app/Fluid.h"

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    }

    const int width  = 1000;
    const int height = 600;

    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

    SDL_Event event;
    bool      running             = true;
    bool      mouse_button_down[] = {false, false};
    bool      mouse_move          = false;
    int       old_mouse_x, old_mouse_y, current_mouse_x, current_mouse_y;

    app::Fluid fluid{};
    while (running) {

        mouse_move = false;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT && !mouse_button_down[0]) {
                        mouse_button_down[0] = true;
                    } else if (event.button.button == SDL_BUTTON_RIGHT && !mouse_button_down[1]) {
                        mouse_button_down[1] = true;
                    }
                    SDL_GetMouseState(&old_mouse_x, &old_mouse_y);
                    current_mouse_x = old_mouse_x;
                    current_mouse_y = old_mouse_y;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT && mouse_button_down[0]) {
                        mouse_button_down[0] = false;
                    } else if (event.button.button == SDL_BUTTON_RIGHT && mouse_button_down[1]) {
                        mouse_button_down[1] = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (mouse_button_down[0] || mouse_button_down[1]) {
                        SDL_GetMouseState(&current_mouse_x, &current_mouse_y);
                        mouse_move = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_SPACE:
                            fluid.clear_previous();
                            fluid.clear_current();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        fluid.clear_previous();
        if (mouse_move) {
            const float mouse_x_in_screen = static_cast<float>(current_mouse_x) / static_cast<float>(width);
            const float mouse_y_in_screen = static_cast<float>(current_mouse_y) / static_cast<float>(height);
            if (mouse_button_down[0]) {
                fluid.add_density(mouse_x_in_screen, mouse_y_in_screen);
            } else {
                fluid.add_density(mouse_x_in_screen, mouse_y_in_screen, -1.0);
            }
            fluid.add_velocity(static_cast<float>(current_mouse_x - old_mouse_x) / static_cast<float>(width),
                               static_cast<float>(current_mouse_y - old_mouse_y) / static_cast<float>(height),
                               mouse_x_in_screen,
                               mouse_y_in_screen);
            old_mouse_x = current_mouse_x;
            old_mouse_y = current_mouse_y;
        }
        fluid.step(0.03);
        glClear(GL_COLOR_BUFFER_BIT);

        int      b = 3;
        SDL_Rect rect;
        rect.w = b;
        rect.h = b;
        for (int w = 0; w < width; w += b) {
            for (int h = 0; h < height; h += b) {
                rect.x       = w;
                rect.y       = h;
                const auto d = std::min(fluid.sample_at(w / static_cast<float>(width), h / static_cast<float>(height)), 2500.0f) / 2500.0;
                SDL_SetRenderDrawColor(renderer, 255 * d, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
