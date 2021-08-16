#include "particleContainer.hpp"

ParticleContainer::ParticleContainer(int size_x, int size_y) : width(size_x), height(size_y) {
    map = new Particle[size_x * size_y];
}

void ParticleContainer::updateAll() {
    static int frameCount = 0;
    frameCount++;
    bool even = frameCount % 2;
    Particle* iter = getMapBegin() + (even ? getMapSize() - 1 : 0);
    for(int y = even ? height - 1 : 0; y != (even ? -1 : height); y += even ? -1 : 1) {
        for(int x = even ? width - 1 : 0; x != (even ? -1 : width); x += even ? -1 : 1) {
            auto update = iter->getUniqueMaterial().update;
            if(update)
                update(this, x, y, even);
            if(even)
                iter--;
            else
                iter++;
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
