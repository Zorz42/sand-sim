#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <vector>

class ParticleContainer;

struct Material {
    std::vector<sf::Color> color;
    float constant_force = 0.08;
    int random_spawn = 0;
    
    virtual void update(ParticleContainer* container, int x, int y, bool even) {}
};

struct Air : Material {
    Air() {
        color = {{90, 90, 90}};
        constant_force = 0;
    }
};

struct Stone : Material {
    Stone() {
        color = {{133, 133, 133}, {135, 135, 135}, {131, 131, 131}};
        constant_force = 0;
    }
};

namespace Materials {
    
inline Material *air = new Air(), *stone = new Stone();

};

class Particle {
public:
    Particle(Material* material) : material(material) {}
    Particle() = default;
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
