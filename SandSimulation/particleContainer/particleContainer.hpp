#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

enum class MaterialType {AIR, SAND, NUM_MATERIALS};

void initMaterials();

class particle_container;

struct Material {
    Material(sf::Color color, short constant_force, void (*update)(particle_container* container, int x, int y, bool even)) :
    color(color), constant_force(constant_force), update(update) {}
    Material() = default;
    sf::Color color;
    short constant_force;
    void (*update)(particle_container* container, int x, int y, bool even);
};

class Particle {
public:
    MaterialType type = MaterialType::AIR;
    float speed_x = 0;
    float speed_y = 0;
    unsigned short timer = 0;
    bool updated = false;
    const Material& getUniqueMaterial();
};

/*class sand: public Material {
public:
    sf::Color getColor() {
        return {237, 205, 88};
    }

    short getConstantForce() {
        return 0;
    }

    void update(particle_container* container, int x, int y, bool even);

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

    void update(particle_container* container, int x, int y, bool even);

    materials getType(){
        return materials::AIR;
    }
};*/

class particle_container{
    Particle* particleMap = nullptr;
    int arraySizeX, arraySizeY;
public:
    void initMap();
    void updateAll();
    particle_container(int x_size, int y_size);
    Particle& getParticle(unsigned short x, unsigned short y);
};
