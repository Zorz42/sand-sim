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

void particle_container::updateAll(particle_container* container) {
    for(int i = 1; i < arraySizeX - 1; i++){
        for(int j = 1; j < arraySizeY - 1; j++){
            getParticle(i, j)->update(container, i, j);
        }
    }
}

Material* particle_container::getParticle(unsigned short x, unsigned short y) {
    return particleMap[y * arraySizeX + x];
}

void sand::update(particle_container* container, int x, int y) {
    //if(container->getParticle()->getType)
}

void air::update(particle_container* container, int x, int y) {
    return;
}
