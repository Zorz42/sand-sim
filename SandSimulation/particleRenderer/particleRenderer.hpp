#pragma once
#include <SFML/Graphics.hpp>
#include "particleContainer.hpp"

class ParticleRenderer {
    sf::RenderWindow* window;
    ParticleContainer* container;
    sf::Clock clock;
    unsigned short width, height;
    sf::Uint8* pixels;
    sf::Texture texture;
    bool left_button_pressed = false, right_button_pressed = false;
    MaterialType selected_material = MaterialType::SAND;
    sf::Text fps_text;
    
    unsigned short getMouseX();
    unsigned short getMouseY();
    
    void updateTexture();
    void renderCircle();
    void placeCircle(MaterialType material_type);
    void renderSelectedMaterial();
public:
    ParticleRenderer(ParticleContainer* container, unsigned short window_width, unsigned short window_height);
    bool isRunning();
    void render();
    ~ParticleRenderer();
};
