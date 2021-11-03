#include "app/Fluid.h"
#include "app/MouseState.h"
#include "app/Shader.h"
#include "app/VertexArray.h"

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <iostream>

int main() {
    sf::ContextSettings settings;
    settings.depthBits    = 24;
    settings.stencilBits  = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 2;

    sf::RenderWindow window(sf::VideoMode(800, 900, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);

    glewExperimental = GL_TRUE;
    glewInit();

    app::VertexArray vertex_array{};
    vertex_array.bind();

    app::Shader shader{};
    shader.init();

    app::MouseState left_click_state{};
    app::MouseState right_click_state{};

    app::Fluid fluid{};
    bool       running      = true;
    auto       current_time = std::chrono::high_resolution_clock::now();
    const auto start_time   = std::chrono::high_resolution_clock::now();
    while (running) {
        shader.update_uniform(std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count());
        sf::Event event{};
        fluid.clear_previous();
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    running = false;
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            running = false;
                            break;
                        case sf::Keyboard::Space:
                            fluid.clear_previous();
                            fluid.clear_current();
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        left_click_state.press();
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        right_click_state.press();
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        left_click_state.release();
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        right_click_state.release();
                    }
                    break;
                case sf::Event::MouseMoved:
                    if (left_click_state.pressed()) {
                        left_click_state.get_new_position(window);
                        fluid.add_density(left_click_state.current());
                        fluid.add_velocity(left_click_state.current(), left_click_state.direction());
                    } else if (right_click_state.pressed()) {
                        right_click_state.get_new_position(window);
                        fluid.add_density(right_click_state.current(), -1.0f);
                        fluid.add_velocity(right_click_state.current(), right_click_state.direction());
                    }
                    break;
                default:
                    break;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        fluid.step(static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(now - current_time).count()) / 1000.0f);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - current_time).count();
        current_time  = now;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        vertex_array.set_color(fluid);
        window.display();

        if (duration != 0) {
            std::cout << "Framerate:\t " << 1000 / duration << "\n";
        }
    }
    window.close();
}
