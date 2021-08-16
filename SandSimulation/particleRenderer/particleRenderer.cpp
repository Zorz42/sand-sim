#include <iostream>
#include "particleRenderer.hpp"
#include <cmath>

#define RADIUS 10

ParticleRenderer::ParticleRenderer(ParticleContainer* container, unsigned short window_width, unsigned short window_height)
: container(container), width(window_width), height(window_height) {
    window = new sf::RenderWindow(sf::VideoMode(width * 2, height * 2), "Sand Simulation", sf::Style::Titlebar | sf::Style::Close);
    window->setVerticalSyncEnabled(true);
    sf::FloatRect visibleArea(0, 0, (unsigned int)width, (unsigned int)height);
    window->setView(sf::View(visibleArea));
    
    pixels = new sf::Uint8[width * height * 4];
    texture.create(width, height);
    //window->setFramerateLimit(60);
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
        else if(event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Key::Num1:
                    selected_material = MaterialType::SAND;
                    break;
                case sf::Keyboard::Key::Num2:
                    selected_material = MaterialType::WATER;
                    break;
                case sf::Keyboard::Key::Num3:
                    selected_material = MaterialType::WOOD;
                    break;

                default:;
            }
        }
    }

    sf::Vector2<int> mouse_position = sf::Mouse::getPosition(*window);
    unsigned short mouse_x = mouse_position.x / 2, mouse_y = mouse_position.y / 2;
    if(button_pressed) {
        for(int x = mouse_x - RADIUS; x < mouse_x + RADIUS; x++)
            for(int y = mouse_y - RADIUS; y < mouse_y + RADIUS; y++)
                if(container->getParticle(x, y).getType() == MaterialType::AIR && rand() % getMaterialByType(selected_material).randomSpawn == 0 && std::pow(x - mouse_x, 2) + std::pow(y - mouse_y, 2) < RADIUS * RADIUS)
                    container->getParticle(x, y).setType(selected_material);
    }

    sf::Uint32* pixel_iter = (sf::Uint32*)pixels;

    Particle* iter = container->getMapBegin();
    for(int i = 0; i < container->getMapSize(); i++)
        if(iter->hasChangedType()) {
            sf::Color color = iter->getUniqueMaterial().color;
            *pixel_iter = color.r | color.g << 8 | color.b << 16 | color.a << 24;
            iter++;
                pixel_iter++;
        }
    
    texture.update(pixels);
    
    window->draw(sf::Sprite(texture));
    
    sf::CircleShape mouse_circle;
    mouse_circle.setPosition(mouse_x - RADIUS, mouse_y - RADIUS);
    mouse_circle.setRadius(RADIUS);
    mouse_circle.setFillColor({50, 50, 50, sf::Uint8(button_pressed ? 150 : 50)});
    mouse_circle.setOutlineThickness(1);
    mouse_circle.setOutlineColor({60, 60, 60});
    window->draw(mouse_circle);
    
    sf::RectangleShape selected_material_rect;
    selected_material_rect.setPosition(5, 5);
    selected_material_rect.setSize(sf::Vector2f(20, 20));
    selected_material_rect.setFillColor(getMaterialByType(selected_material).color);
    selected_material_rect.setOutlineColor({50, 50, 50});
    selected_material_rect.setOutlineThickness(1);
    window->draw(selected_material_rect);
    
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
