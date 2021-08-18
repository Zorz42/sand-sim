#include <iostream>
#include "particleRenderer.hpp"
#include <cmath>

#define RADIUS 10

ParticleRenderer::ParticleRenderer(ParticleContainer* container, unsigned short window_width, unsigned short window_height)
: container(container), width(window_width), height(window_height) {
    window = new sf::RenderWindow(sf::VideoMode(width * 2, height * 2), "Sand Simulation", sf::Style::Titlebar | sf::Style::Close);
    //window->setVerticalSyncEnabled(true);
    sf::FloatRect visibleArea(0, 0, (unsigned int)width, (unsigned int)height);
    window->setView(sf::View(visibleArea));
    window->setFramerateLimit(60);
    
    pixels = new sf::Uint8[width * height * 4];
    texture.create(width, height);
    
    fps_text.setFillColor({0, 0, 0});
    fps_text.setPosition(0, 0);
}

short ParticleRenderer::getMouseX() {
    return sf::Mouse::getPosition(*window).x / 2;
}

short ParticleRenderer::getMouseY() {
    return sf::Mouse::getPosition(*window).y / 2;
}

void ParticleRenderer::updateTexture() {
    sf::Uint32* pixel_iter = (sf::Uint32*)pixels;
    Particle* iter = container->getMapBegin();
    for(int i = 0; i < container->getMapSize(); i++)
    if(iter->hasChangedType()) {
        sf::Color color = iter->getMaterial()->color[iter->textureColor % iter->getMaterial()->color.size()];
        *pixel_iter = color.r | color.g << 8 | color.b << 16 | color.a << 24;
        iter++;
        pixel_iter++;
    }
    
    texture.update(pixels);
}

void ParticleRenderer::renderCircle() {
    sf::CircleShape mouse_circle;
    mouse_circle.setPosition(getMouseX() - RADIUS, getMouseY() - RADIUS);
    mouse_circle.setRadius(RADIUS);
    mouse_circle.setFillColor({50, 50, 50, sf::Uint8(left_button_pressed ? 150 : 50)});
    mouse_circle.setOutlineThickness(1);
    mouse_circle.setOutlineColor({60, 60, 60});
    window->draw(mouse_circle);
}

void ParticleRenderer::placeCircle(short target_x, short target_y, Material* material, int line_length) {
    for(int x = target_x - RADIUS; x < target_x + RADIUS; x++)
        for(int y = target_y - RADIUS; y < target_y + RADIUS; y++)
            if(
               x >= 0 && x < width && y >= 0 && y < height &&
               (container->getParticle(x, y).getMaterial() == Materials::air || material == Materials::air)
               && rand() % (material->randomSpawn * line_length + 1) == 0 &&
               std::pow(x - target_x, 2) + std::pow(y - target_y, 2) < RADIUS * RADIUS
               )
                container->getParticle(x, y).setMaterial(material);
}

void ParticleRenderer::placeCirclesFromTo(short x1, short y1, short x2, short y2, Material* material) {
    int length = std::sqrt(std::pow(abs(x1 - x2), 2) + std::pow(abs(y1 - y2), 2)) + 1;
    
    int dx = x2 - x1;
    int ix = (dx > 0) - (dx < 0);

    dx = abs(dx) * 2;

    int dy = y2 - y1;
    int iy = (dy > 0) - (dy < 0);
    dy = abs(dy) * 2;

    placeCircle(x1, y1, material, length);

    if(dx >= dy) {
        int error = dy - dx / 2;

        while(x1 != x2) {
            if(error >= 0 && (error || ix > 0)) {
                error -= dx;
                y1 += iy;
            }

            error += dy;
            x1 += ix;

            placeCircle(x1, y1, material, length);
        }
    } else {
        int error = dx - dy / 2;

        while(y1 != y2) {
            if(error >= 0 && (error || iy > 0)) {
                error -= dy;
                x1 += ix;
            }

            error += dx;
            y1 += iy;

            placeCircle(x1, y1, material, length);
        }
    }
}

void ParticleRenderer::renderSelectedMaterial() {
    sf::RectangleShape selected_material_rect;
    selected_material_rect.setPosition(5, 5);
    selected_material_rect.setSize(sf::Vector2f(20, 20));
    selected_material_rect.setFillColor(selected_material->color[0]);
    selected_material_rect.setOutlineColor({50, 50, 50});
    selected_material_rect.setOutlineThickness(1);
    window->draw(selected_material_rect);
}

void ParticleRenderer::render() {
    sf::Event event;
    while(window->pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            window->close();
        else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
            left_button_pressed = true;
        else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
            left_button_pressed = false;
        else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right)
            right_button_pressed = true;
        else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Right)
            right_button_pressed = false;
        else if(event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Key::Num1:
                    selected_material = Materials::sand;
                    break;
                case sf::Keyboard::Key::Num2:
                    selected_material = Materials::water;
                    break;
                case sf::Keyboard::Key::Num3:
                    selected_material = Materials::wood;
                    break;
                case sf::Keyboard::Key::Num4:
                    selected_material = Materials::lava;
                    break;
                case sf::Keyboard::Key::Num5:
                    selected_material = Materials::stone;
                    break;
                case sf::Keyboard::Key::Num6:
                    selected_material = Materials::gunpowder;
                    break;
                case sf::Keyboard::Key::Num7:
                    selected_material = Materials::acid;
                    break;
                case sf::Keyboard::Key::Num8:
                    selected_material = Materials::americaWantsIt;
                    break;
                default:;
            }
        }
    }
    
    static short prev_mouse_x = 0, prev_mouse_y = 0;
    if(left_button_pressed)
        placeCirclesFromTo(prev_mouse_x, prev_mouse_y, getMouseX(), getMouseY(), selected_material);
    else if(right_button_pressed)
        placeCirclesFromTo(prev_mouse_x, prev_mouse_y, getMouseX(), getMouseY(), Materials::air);
    prev_mouse_x = getMouseX();
    prev_mouse_y = getMouseY();
    
    updateTexture();
    window->draw(sf::Sprite(texture));
    
    renderCircle();
    renderSelectedMaterial();
    window->display();
}

bool ParticleRenderer::isRunning() {
    return window->isOpen();
}

ParticleRenderer::~ParticleRenderer() {
    delete[] pixels;
}
