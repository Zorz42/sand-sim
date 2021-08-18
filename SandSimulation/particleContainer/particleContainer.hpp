#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <vector>

struct Material;

void initMaterials();

class ParticleContainer;

struct Material {
    Material(std::vector<sf::Color> color, float constant_force, int random_spawn);
    const std::vector<sf::Color> color;
    const float constant_force;
    const int random_spawn;
    virtual void update(ParticleContainer* container, int x, int y, bool even) {}
};

namespace Materials {
    
inline Material *air, *sand, *water, *wood, *fire, *stone, *smoke, *gunpowder, *acid, *oil, *lava;

};

class Particle {
    Material* material = Materials::air;
    bool changed_type = true;
public:
    Particle(Material* material) : material(material) {}
    Particle() = default;
    int texture_color = rand();
    float speed_x = 0, speed_y = 0;
    unsigned short timer = 0;
    bool updated = false;
    Material* getMaterial();
    void setMaterial(Material* material_to_be);
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

struct Air : Material {
    Air() : Material({{90, 90, 90}}, 0, 0) {}
};

struct Sand : Material {
    Sand() : Material({{245, 210, 92}, {237, 205, 88}, {239, 207, 90}, {235, 203, 86}, {230, 200, 84}}, 0.08, 80) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Water : Material {
    Water() : Material({{52, 145, 173}, {53, 147, 176}, {47, 140, 169}}, 0.08, 80) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Wood : Material {
    Wood() : Material({{155, 116, 54}, {154, 115, 53}, {156, 117, 55}, {150, 111, 51}}, 0, 0) {}
};

struct Fire : Material {
    Fire() : Material({{222, 70, 12}, {222, 120, 16}, {222, 125, 16}, {222, 127, 16}, {222, 130, 16}, {222, 182, 16}}, 0, 20) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Stone : Material {
    Stone() : Material({{133, 133, 133}, {135, 135, 135}, {131, 131, 131}}, 0, 0) {}
};

struct Smoke : Material {
    Smoke() : Material({{65, 65, 59}, {60, 60, 54}, {70, 70, 64}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Gunpowder : Material {
    Gunpowder() : Material({{36, 36, 36}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Acid : Material {
    Acid() : Material({{31, 255, 61}, {32, 255, 62}, {31, 255, 61}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Oil : Material {
    Oil() : Material({{10, 10, 10}, {12, 12, 12}, {9, 9, 9}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Lava : Material {
    Lava() : Material({{252, 91, 16}, {252, 95, 16}, {252, 97, 16}, {252, 102, 16}, {252, 142, 16}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};
