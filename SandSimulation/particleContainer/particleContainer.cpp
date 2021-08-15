#include "particleContainer.hpp"


void particle_container::initMap() {
    for(int i = 0; i < arraySizeX * arraySizeY; i++) {
        particleMap[i] = i > 100000 ? (Material*)new air : (Material*)new sand;
    }
}

particle_container::particle_container(int x_size, int y_size) {
    particleMap = new Material*[x_size * y_size];
    arraySizeX = x_size;
    arraySizeY = y_size;
    initMap();
}

void particle_container::updateAll() {
    static bool even = false;
    even = !even;
    for(int i = 1; i < arraySizeX - 1; i++){
        for(int j = 1; j < arraySizeY - 1; j++){
            getParticle(i, j)->update(this, i, j, even);
        }
    }
}

Material*& particle_container::getParticle(unsigned short x, unsigned short y) {
    return particleMap[y * arraySizeX + x];
}

void sand::update(particle_container* container, int x, int y, bool even) {
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
}
