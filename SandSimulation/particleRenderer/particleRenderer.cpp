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
    
    sf::Uint8 pixels[width * height * 4];
    
    Particle* iter = container->map;
    for(int i = 0; i < width * height; i++) {
        sf::Color color = iter->getUniqueMaterial().color;
        int index = i * 4;
        pixels[index]     = color.r;
        pixels[index + 1] = color.g;
        pixels[index + 2] = color.b;
        pixels[index + 3] = color.a;
        iter++;
    }
    
    sf::Image image;
    image.create(width, height, pixels);
    
    sf::Texture texture;
    texture.loadFromImage(image);
    
    window->draw(sf::Sprite(texture));
    window->display();
    
    float currentTime = clock.restart().asSeconds();
    float fps = 1.f / currentTime;
    lastTime = currentTime;
    
    std::cout << "FPS: " << fps << std::endl;
}

bool ParticleRenderer::isRunning() {
    return window->isOpen();
}
