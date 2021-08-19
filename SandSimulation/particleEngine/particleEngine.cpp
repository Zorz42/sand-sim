#include "particleEngine.hpp"

ParticleContainer::ParticleContainer(int size_x, int size_y) : width(size_x), height(size_y) {
    map = new Particle[size_x * size_y];
}

void ParticleContainer::updateAll() {
    static int frameCount = 0;
    frameCount++;
    bool even = frameCount % 2;
    Particle* iter = getMapBegin() + getMapSize() - (even ? 1 : width);
    for(int y = height - 1; y >= 0; y--) {
        for(int x = even ? width - 1 : 0; x != (even ? -1 : width); x += even ? -1 : 1) {
            iter->update(this, x, y, even);
            if(even)
                iter--;
            else
                iter++;
        }
        if(!even)
            iter -= width * 2;
    }
}

Particle& ParticleContainer::getParticle(unsigned short x, unsigned short y) {
    static Particle out_of_bounds(Materials::stone);
    if(x < 0 || x >= width || y < 0 || y >= height)
        return out_of_bounds;
    return map[y * width + x];
}

unsigned int ParticleContainer::getMapSize() {
    return width * height;
}

void Particle::update(ParticleContainer* container, int x, int y, bool even) {
    speed_y *= 0.995;
    speed_x *= 0.995;
    speed_y += material->constant_force;
    material->update(container, x, y, even);
}

Air::Air() {
    color = {{90, 90, 90}};
    constant_force = 0;
    type = MaterialType::SOLID;
}

Stone::Stone() {
    color = {{133, 133, 133}, {135, 135, 135}, {131, 131, 131}};
    constant_force = 0;
    type = MaterialType::SOLID;
}
