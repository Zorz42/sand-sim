#include "particleContainer.hpp"


void particle_container::initMap() {
    for(int i = 0; i < arraySizeX * arraySizeY; i++){
        particleMap[i] = new air;
    }
}

particle_container::particle_container(int x_size, int y_size) {
    particleMap = new Material*[x_size * y_size];
    arraySizeX = x_size;
    arraySizeY = y_size;
    initMap();
}

void particle_container::updateAll() {
    for(int i = 0; i < arraySizeX * arraySizeY; i++){
        particleMap[i]->update();
    }
}

Material* particle_container::getParticle(unsigned short x, unsigned short y) {
    return particleMap[y * arraySizeX + x];
}
