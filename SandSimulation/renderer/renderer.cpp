#include "renderer.hpp"

Renderer::Renderer(unsigned short window_width, unsigned short window_height) {
    window = new sf::RenderWindow(sf::VideoMode(window_width, window_height), "Sand Simulation", sf::Style::Titlebar | sf::Style::Close);
}

void Renderer::render() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();
    }

    window->clear({255, 255, 255});
    
    window->display();
}

bool Renderer::isRunning() {
    return window->isOpen();
}
