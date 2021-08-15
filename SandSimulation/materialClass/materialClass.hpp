#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

struct Material {
    virtual void update() = 0;
    virtual sf::Color getColor() = 0;
    virtual short getConstantForce() = 0;
    float speed_x = 0;
    float speed_y = 0;
    unsigned short timer = 0;
    virtual ~Material() {}
};

class sand: public Material {
public:
    sf::Color getColor() {
        return {237, 205, 88};
    }

    short getConstantForce() {
        return 10;
    }

    void update();
};

class air: public Material{
public:
    sf::Color getColor() {
        return {0, 0, 0};
    }

    short getConstantForce() {
        return 10;
    }

    void update();
};
