#pragma once
#include <SFML/Graphics.hpp>
#include "particles.hpp"
#include "shaders.hpp"


class ParticleRenderer {
    sf::RenderWindow* window;
    ParticleContainer* container;
    sf::Clock clock;
    unsigned short width, height;
    sf::Uint8* pixels;
    sf::Texture texture;
    sf::RenderTexture bloom_mask_texture;
    bool left_button_pressed = false, right_button_pressed = false;
    Material* selected_material = Materials::sand;
    sf::Text fps_text;
    bool is_paused = false;
    
    short getMouseX();
    short getMouseY();
    
    void updateTexture();
    void renderCircle();
    void placeCircle(short target_x, short target_y, Material* material, int line_length);
    void placeCirclesFromTo(short x1, short y1, short x2, short y2, Material* material);
    void renderSelectedMaterial();
    void renderPauseRect();
    void updateBloomMask();
public:
    ParticleRenderer(ParticleContainer* container, unsigned short window_width, unsigned short window_height);
    bool isRunning();
    bool isPaused();
    
    void render();
    ~ParticleRenderer();
    void applyShader(const sf::Shader& shader, sf::RenderTexture& output);
};
