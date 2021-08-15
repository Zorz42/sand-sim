#include <iostream>
#include "particleRenderer.hpp"

ParticleRenderer::ParticleRenderer(particle_container* container, unsigned short window_width, unsigned short window_height)
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
    
    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++) {
            Particle& particle = container->getParticle(x, y);
            sf::Color color = particle.getUniqueMaterial().color;
            int index = (y * width + x) * 4;
            pixels[index]     = color.r;
            pixels[index + 1] = color.g;
            pixels[index + 2] = color.b;
            pixels[index + 3] = color.a;
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
