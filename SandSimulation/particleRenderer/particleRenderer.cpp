#include <iostream>
#include "particleRenderer.hpp"

ParticleRenderer::ParticleRenderer(ParticleContainer* container, unsigned short window_width, unsigned short window_height)
: container(container), width(window_width), height(window_height) {
    window = new sf::RenderWindow(sf::VideoMode(width * 2, height * 2), "Sand Simulation", sf::Style::Titlebar | sf::Style::Close);
    window->setVerticalSyncEnabled(true);
    sf::FloatRect visibleArea(0, 0, (unsigned int)width, (unsigned int)height);
    window->setView(sf::View(visibleArea));
    
    pixels = new sf::Uint8[width * height * 4];
    texture.create(width, height);
    window->setFramerateLimit(60);
}

void ParticleRenderer::render() {
    sf::Event event;
    while(window->pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            window->close();
        else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
            button_pressed = true;
        else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
            button_pressed = false;
    }
    
    if(button_pressed) {
        sf::Vector2<int> mouse_position = sf::Mouse::getPosition(*window);
        unsigned short mouse_x = mouse_position.x / 2, mouse_y = mouse_position.y / 2;
        container->getParticle(mouse_x, mouse_y).type = MaterialType::SAND;
        
    }
    
    sf::Uint32* pixel_iter = (sf::Uint32*)pixels;
    
    Particle* iter = container->getMapBegin();
    for(int i = 0; i < container->getMapSize(); i++) {
        sf::Color color = iter->getUniqueMaterial().color;
        *pixel_iter = color.r | color.g << 8 | color.b << 16 | color.a << 24;
        iter++;
        pixel_iter++;
    }
    
    texture.update(pixels);
    
    window->draw(sf::Sprite(texture));
    window->display();
    
    static unsigned short frame_count = 0;
    frame_count++;
    if(clock.getElapsedTime().asSeconds() > 1) {
        std::cout << "FPS: " << frame_count << std::endl;
        clock.restart();
        frame_count = 0;
    }
}

bool ParticleRenderer::isRunning() {
    return window->isOpen();
}

ParticleRenderer::~ParticleRenderer() {
    delete[] pixels;
}
