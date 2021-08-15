#pragma once
#include <SFML/Graphics.hpp>
#include "particleContainer.hpp"

class ParticleRenderer {
    sf::RenderWindow* window;
    ParticleContainer* container;
    sf::Clock clock;
    unsigned short width, height;
public:
    ParticleRenderer(ParticleContainer* container, unsigned short window_width, unsigned short window_height);
    bool isRunning();
    void render();
};
