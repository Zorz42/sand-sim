#include <vector>
#include "particleContainer.hpp"

Material materials[(int)MaterialType::NUM_MATERIALS];

void initMaterials() {
    materials[(int)MaterialType::AIR] = Material({90, 90, 90}, 0, [](particle_container* container, int x, int y, bool even) {
        
    });
    
    materials[(int)MaterialType::SAND] = Material({237, 205, 88}, 0, [](particle_container* container, int x, int y, bool even) {
        if(container->getParticle(x, y).updated == even && container->getParticle(x, y + 1).type == MaterialType::AIR) {
            Particle temporary_particle = container->getParticle(x, y);
            container->getParticle(x, y) = container->getParticle(x, y + 1);
            container->getParticle(x, y + 1) = temporary_particle;
            container->getParticle(x, y + 1).updated = !even;
        }
        container->getParticle(x, y).updated = !even;
    });
}

particle_container::particle_container(int x_size, int y_size) {
    particleMap = new Particle[x_size * y_size];
    arraySizeX = x_size;
    arraySizeY = y_size;
}

void particle_container::updateAll() {
    static bool even = false;
    even = !even;
    for(int x = 1; x < arraySizeX - 1; x++){
        for(int y = 1; y < arraySizeY - 1; y++){
            Particle& particle = getParticle(x, y);
            particle.getUniqueMaterial().update(this, x, y, even);
        }
    }
}

Particle& particle_container::getParticle(unsigned short x, unsigned short y) {
    return particleMap[y * arraySizeX + x];
}

/*void sand::update(particle_container* container, int x, int y, bool even) {
    if(container->getParticle(x, y)->updated == even && container->getParticle(x, y + 1)->getType() == materials::AIR){
        Material* temporary_ponter = container->getParticle(x, y);
        container->getParticle(x, y) = container->getParticle(x, y + 1);
        container->getParticle(x, y + 1) = temporary_ponter;
        container->getParticle(x, y + 1)->updated = !even;
    }
    container->getParticle(x, y)->updated = !even;
}

void air::update(particle_container* container, int x, int y, bool even) {
    return;
}*/

const Material& Particle::getUniqueMaterial() {
    return materials[(int)type];
}
