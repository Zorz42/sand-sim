#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

enum class MaterialType {AIR, SAND, NUM_MATERIALS};

void initMaterials();

class ParticleContainer;

struct Material {
    Material(sf::Color color, short constant_force, void (*update)(ParticleContainer* container, int x, int y, bool even)=nullptr) :
    color(color), constant_force(constant_force), update(update) {}
    Material() = default;
    sf::Color color;
    short constant_force;
    void (*update)(ParticleContainer* container, int x, int y, bool even);
};

class Particle {
public:
    Particle(MaterialType type) : type(type) {}
    Particle() = default;
    MaterialType type = MaterialType::AIR;
    float speed_x = 0;
    float speed_y = 0;
    unsigned short timer = 0;
    bool updated = false;
    const Material& getUniqueMaterial();
};

class ParticleContainer {
    int size_x, size_y;
public:
    Particle* map = nullptr;
    void initMap();
    void updateAll();
    MaterialType getType(short x, short y);
    ParticleContainer(int size_x, int size_y);
    Particle& getParticle(unsigned short x, unsigned short y);
};
