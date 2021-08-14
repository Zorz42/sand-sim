#pragma once
#include <SFML/Graphics.hpp>

class Renderer {
    sf::RenderWindow* window;
public:
    void init();
    bool isOpen();
    void render();
};
