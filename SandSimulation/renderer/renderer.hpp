#pragma once
#include <SFML/Graphics.hpp>

class Renderer {
    sf::RenderWindow* window;
public:
    Renderer(unsigned short window_width, unsigned short window_height);
    bool isRunning();
    void render();
};
