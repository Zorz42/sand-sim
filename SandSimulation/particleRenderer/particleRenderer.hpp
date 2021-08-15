#pragma once
#include <SFML/Graphics.hpp>
#include "particleContainer.hpp"

class ParticleRenderer {
    sf::RenderWindow* window;
    particle_container* container;
    sf::Clock clock;
    unsigned short width, height;
public:
    ParticleRenderer(particle_container* container, unsigned short window_width, unsigned short window_height);
    bool isRunning();
    void render();
};
