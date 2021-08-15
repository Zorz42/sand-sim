#pragma once
#include <SFML/Graphics.hpp>

class ParticleRenderer {
    sf::RenderWindow* window;
public:
    ParticleRenderer(unsigned short window_width, unsigned short window_height);
    bool isRunning();
    void render();
};
