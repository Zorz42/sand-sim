#include <vector>
#include "particleContainer.hpp"

Material materials[(int)MaterialType::NUM_MATERIALS];

void initMaterials() {
    materials[(int)MaterialType::AIR] = Material({90, 90, 90}, 0);

    materials[(int)MaterialType::SAND] = Material({237, 205, 88}, 1, [](ParticleContainer* container, int x, int y, bool even) {
        container->getParticle(x, y).speed_y *= 0.9;
        container->getParticle(x, y).speed_x *= 0.9;
        container->getParticle(x, y).speed_y += container->getParticle(x, y).getUniqueMaterial().constant_force;
        int i = 0;
        do{
            if(container->getParticle(x, y).updated == even && container->getParticle(x, y + 1).type == MaterialType::AIR) {
                Particle temporary_particle = container->getParticle(x, y);
                container->getParticle(x, y) = container->getParticle(x, y + 1);
                container->getParticle(x, y + 1) = temporary_particle;
                if(i + 1 > container->getParticle(x, y + 1).speed_y)
                    container->getParticle(x, y + 1).updated = !even;
                y++;
            }else if(container->getParticle(x, y).updated == even && container->getParticle(x + 1, y + 1).type == MaterialType::AIR) {
                Particle temporary_particle = container->getParticle(x, y);
                container->getParticle(x, y) = container->getParticle(x + 1, y + 1);
                container->getParticle(x + 1, y + 1) = temporary_particle;
                if(i + 1 > container->getParticle(x, y).speed_y)
                    container->getParticle(x + 1, y + 1).updated = !even;
                y++;
                x++;
            }else if(container->getParticle(x, y).updated == even && container->getParticle(x - 1, y + 1).type == MaterialType::AIR) {
                Particle temporary_particle = container->getParticle(x, y);
                container->getParticle(x, y) = container->getParticle(x - 1, y + 1);
                container->getParticle(x - 1, y + 1) = temporary_particle;
                if(i + 1 > container->getParticle(x, y).speed_y)
                    container->getParticle(x - 1, y + 1).updated = !even;
                y++;
                x--;
            }else{
                container->getParticle(x, y).speed_y = 0;
                break;
            }
            i++;
        }
        while(i < container->getParticle(x, y).speed_y);
    });
}

ParticleContainer::ParticleContainer(int size_x, int size_y) : width(size_x), height(size_y) {
    map = new Particle[size_x * size_y];
}

void ParticleContainer::updateAll() {
    static bool even = false;
    even = !even;
    Particle* iter = getMapBegin();
    for(int i = 0; i < getMapSize(); i++) {
        auto update = iter->getUniqueMaterial().update;
        if(update)
            update(this, i % width, i / width, even);
        iter++;
        if(((i % width) - 620) * ((i % width) - 500) + ((i / width) - 400) * ((i / width) - 400) < 200)
            *iter = Particle(MaterialType::SAND);
    }
}

Particle& ParticleContainer::getParticle(unsigned short x, unsigned short y) {
    static Particle out_of_bounds;
    if(x < 0 || x >= width || y < 0 || y >= height)
        return out_of_bounds;
    return map[y * width + x];
}

const Material& Particle::getUniqueMaterial() {
    return materials[(int)type];
}

unsigned int ParticleContainer::getMapSize() {
    return width * height;
}
