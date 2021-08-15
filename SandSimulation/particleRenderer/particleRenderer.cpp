#include "particleRenderer.hpp"

ParticleRenderer::ParticleRenderer(unsigned short window_width, unsigned short window_height) {
    window = new sf::RenderWindow(sf::VideoMode(window_width, window_height), "Sand Simulation", sf::Style::Titlebar | sf::Style::Close);
}

void ParticleRenderer::render() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();
    }

    window->clear({255, 255, 255});
    
    window->display();
}

bool ParticleRenderer::isRunning() {
    return window->isOpen();
}
