#include <vector>
#include "particleContainer.hpp"

Material materials[(int)MaterialType::NUM_MATERIALS];

void initMaterials() {
    materials[(int)MaterialType::AIR] = Material({90, 90, 90}, 0);
    
    materials[(int)MaterialType::SAND] = Material({237, 205, 88}, 0, [](ParticleContainer* container, int x, int y, bool even) {
        if(container->getParticle(x, y).updated == even && container->getParticle(x, y + 1).type == MaterialType::AIR) {
            Particle temporary_particle = container->getParticle(x, y);
            container->getParticle(x, y) = container->getParticle(x, y + 1);
            container->getParticle(x, y + 1) = temporary_particle;
            container->getParticle(x, y + 1).updated = !even;
        }
        container->getParticle(x, y).updated = !even;
    });
}

ParticleContainer::ParticleContainer(int size_x, int size_y) : size_x(size_x), size_y(size_y) {
    map = new Particle[size_x * size_y];
}

void ParticleContainer::updateAll() {
    static bool even = false;
    even = !even;
    Particle* iter = map;
    for(int i = 0; i < size_x * size_y; i++) {
        auto update = iter->getUniqueMaterial().update;
        if(update)
            update(this, i % size_x, i / size_x, even);
        iter++;
    }
}

Particle& ParticleContainer::getParticle(unsigned short x, unsigned short y) {
    static Particle out_of_bounds;
    if(x < 0 || x >= size_x || y < 0 || y >= size_y)
        return out_of_bounds;
    return map[y * size_x + x];
}

const Material& Particle::getUniqueMaterial() {
    return materials[(int)type];
}
