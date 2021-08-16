#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

enum class MaterialType {AIR, SAND, WATER, WOOD, FIRE, STONE, NUM_MATERIALS};

struct Material;

void initMaterials();
const Material& getMaterialByType(MaterialType type);

class ParticleContainer;

struct Material {
    Material(sf::Color color, float constant_force, int randomSpawn, void (*update)(ParticleContainer* container, int x, int y, bool even)=nullptr);
    Material() = default;
    sf::Color color;
    float constant_force;
    int randomSpawn;
    void (*update)(ParticleContainer* container, int x, int y, bool even) = nullptr;
};

class Particle {
    MaterialType type = MaterialType::AIR;
    bool changed_type = true;
public:
    Particle(MaterialType type) : type(type) {}
    Particle() = default;
    MaterialType getType();
    void setType(MaterialType material_type);
    float speed_x = 0, speed_y = 0;
    unsigned short timer = 0;
    bool updated = false;
    const Material& getUniqueMaterial();
    void markUpdated();
    bool hasChangedType();
    
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
