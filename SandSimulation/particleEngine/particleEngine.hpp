#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <vector>

enum class MaterialType {SOLID, POWDER, LIQUID, GAS};

class ParticleContainer;

struct Material {
    std::vector<sf::Color> color;
    int random_spawn = 0;
    MaterialType type;

    virtual void update(ParticleContainer* container, int x, int y, bool even) {}
};

struct Air : Material {
    Air();
};

struct Stone : Material {
    Stone();
};

namespace Materials {
    
inline Material *air = new Air(), *stone = new Stone();

};

class Particle {
public:
    Particle(Material* material) : material(material) {}
    Particle() = default;
    void update(ParticleContainer* container, int x, int y, bool even);
    Material* material = Materials::air;
    int texture_color = rand();
    float speed_x = 0, speed_y = 0;
    unsigned short timer = 0;
    bool updated = false;
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

void swapParticles(Particle& particle1, Particle& particle2);
