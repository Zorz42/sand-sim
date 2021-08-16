#include "particleContainer.hpp"

ParticleContainer::ParticleContainer(int size_x, int size_y) : width(size_x), height(size_y) {
    map = new Particle[size_x * size_y];
}

void ParticleContainer::updateAll() {
    static bool even = false;
    static int frameCount = 0;
    frameCount++;
    even = !even;
    Particle* iter = getMapBegin() + getMapSize() - 1;
    for(int y = height - 1; y >= 0; y--) {
        for(int x = width - 1; x >= 0; x--) {
            auto update = iter->getUniqueMaterial().update;
            if(update)
                update(this, x, y, even);
            iter--;
        }
    }
}

Particle& ParticleContainer::getParticle(unsigned short x, unsigned short y) {
    static Particle out_of_bounds(MaterialType::STONE);
    if(x < 0 || x >= width || y < 0 || y >= height)
        return out_of_bounds;
    return map[y * width + x];
}

unsigned int ParticleContainer::getMapSize() {
    return width * height;
}
