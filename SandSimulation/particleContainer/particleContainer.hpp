#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

enum class MaterialType {AIR, SAND, WATER, WOOD, FIRE, STONE, NUM_MATERIALS};

struct Material;

void initMaterials();
const Material& getMaterialByType(MaterialType type);

class ParticleContainer;

struct Material {
    Material(sf::Color color, float constant_force, int randomSpawn, void (*update)(ParticleContainer* container, int x, int y, bool even)=nullptr) :
    color(color), constant_force(constant_force), randomSpawn(randomSpawn), update(update) {}
    Material() = default;
    sf::Color color;
    float constant_force;
    int randomSpawn;
    void (*update)(ParticleContainer* container, int x, int y, bool even);
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

bool sandSwapLeftDown(int x, int y, ParticleContainer* container, bool even, int i);
bool sandSwapRightDown(int& x, int& y, ParticleContainer* container, bool even, int i);
bool waterSwapLeftDown(int x, int y, ParticleContainer* container, bool even, int i);
bool waterSwapRightDown(int& x, int& y, ParticleContainer* container, bool even, int i);
bool waterSwapLeft(int x, int y, ParticleContainer* container, bool even, int i);
bool waterSwapRight(int& x, int& y, ParticleContainer* container, bool even, int i);
