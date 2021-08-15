#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

enum class materials {AIR, SAND};

class particle_container;

struct Material {
    virtual void update(particle_container* container, int x, int y) = 0;
    virtual sf::Color getColor() = 0;
    virtual short getConstantForce() = 0;
    virtual materials getType() = 0;
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
        return 0;
    }

    void update(particle_container* container, int x, int y);

    materials getType(){
        return materials::SAND;
    }
};

class air: public Material{
public:
    sf::Color getColor() {
        return {90, 90, 90};
    }

    short getConstantForce() {
        return 0;
    }

    void update(particle_container* container, int x, int y);

    materials getType(){
        return materials::AIR;
    }
};

class particle_container{
    Material** particleMap = nullptr;
    int arraySizeX, arraySizeY;
public:
    void initMap();
    void updateAll();
    particle_container(int x_size, int y_size);
    Material*& getParticle(unsigned short x, unsigned short y);
};
