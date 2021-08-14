#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

struct particle {
    float speed_x;
    float speed_y;
    int timer;
};

struct material {
    sf::Color color;//this will probably get changed from vector
    short constant_force;
};

class sand:material {
public:
    sand() {
        constant_force = 10;
        color = {237, 205, 88};
    }
    void update();
};

class air:material{
public:
    air(){
        constant_force = 10;
        color = {0, 0, 0, 0};
    }
    void update();
};
