#include <iostream>
#include "particleRenderer.hpp"

ParticleRenderer::ParticleRenderer(ParticleContainer* container, unsigned short window_width, unsigned short window_height)
: container(container), width(window_width), height(window_height) {
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Sand Simulation", sf::Style::Titlebar | sf::Style::Close);
    window->setVerticalSyncEnabled(true);
}

void ParticleRenderer::render() {
    static float lastTime = 0;
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();
    }
    
    sf::Uint8* pixels = new sf::Uint8[width * height * 4];
    sf::Uint32* pixel_iter = (sf::Uint32*)pixels;
    
    Particle* iter = container->getMapBegin();
    for(int i = 0; i < container->getMapSize(); i++) {
        sf::Color color = iter->getUniqueMaterial().color;
        *pixel_iter = color.r | color.g << 8 | color.b << 16 | color.a << 24;
        iter++;
        pixel_iter++;
    }
    
    sf::Texture texture;
    texture.create(width, height);
    texture.update(pixels);
    
    window->draw(sf::Sprite(texture));
    window->display();
    
    delete[] pixels;
    
    float currentTime = clock.restart().asSeconds();
    float fps = 1.f / currentTime;
    lastTime = currentTime;
    
    std::cout << "FPS: " << fps << std::endl;
}

bool ParticleRenderer::isRunning() {
    return window->isOpen();
}
