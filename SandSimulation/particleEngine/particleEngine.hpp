#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <vector>

class ParticleContainer;

struct Material {
    Material(std::vector<sf::Color> color, float constant_force, int random_spawn);
    const std::vector<sf::Color> color;
    const float constant_force;
    const int random_spawn;
    virtual void update(ParticleContainer* container, int x, int y, bool even) {}
};

struct Air : Material {
    Air() : Material({{90, 90, 90}}, 0, 0) {}
};

struct Stone : Material {
    Stone() : Material({{133, 133, 133}, {135, 135, 135}, {131, 131, 131}}, 0, 0) {}
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
