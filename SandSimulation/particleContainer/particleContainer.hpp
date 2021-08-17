#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <vector>

struct Material;

void initMaterials();

class ParticleContainer;

struct Material {
    Material(std::vector<sf::Color> color, float constant_force, int randomSpawn, void (*update)(ParticleContainer* container, int x, int y, bool even)=nullptr);
    Material() = default;
    std::vector<sf::Color> color;
    float constant_force;
    int randomSpawn;
    void (*update)(ParticleContainer* container, int x, int y, bool even) = nullptr;
};

namespace Materials {
    inline Material air, sand, water, wood, fire, stone, smoke, gunpowder, acid, americaWantsIt, lava;
};

class Particle {
    Material* material = &Materials::air;
    bool changed_type = true;
public:
    Particle(Material* material) : material(material) {}
    Particle() = default;
    Material* getMaterial();
    void setMaterial(Material* material_to_be);
    float speed_x = 0, speed_y = 0;
    unsigned short timer = 0;
    bool updated = false;
    void markUpdated();
    bool hasChangedType();
    int textureColor = rand();
    void operator=(Particle& x);
};

class ParticleContainer {
    int width, height;
    Particle* map = nullptr;
public:
    ParticleContainer(int size_x, int size_y);
    Particle* getMapBegin() { return map; }
    unsigned int getMapSize();
    void initMap();
    void updateAll();
    Particle& getParticle(unsigned short x, unsigned short y);
};
