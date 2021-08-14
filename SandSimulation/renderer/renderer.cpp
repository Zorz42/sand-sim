#include "renderer.hpp"

void Renderer::init() {
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "Sand Simulation");
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

bool Renderer::isOpen() {
    return window->isOpen();
}
