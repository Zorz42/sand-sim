#include <vector>
#include "particleContainer.hpp"

Material materials[(int)MaterialType::NUM_MATERIALS];

void initMaterials() {
    materials[(int)MaterialType::AIR]   = Material({90, 90, 90}, 0);

    materials[(int)MaterialType::SAND]  = Material({237, 205, 88}, 1.5, [](ParticleContainer* container, int x, int y, bool even) {
        container->getParticle(x, y).speed_y *= 0.95;
        container->getParticle(x, y).speed_x *= 0.95;
        container->getParticle(x, y).speed_y += container->getParticle(x, y).getUniqueMaterial().constant_force;
        int i = 0;
        do{
            if(container->getParticle(x, y).updated == even && (container->getParticle(x, y + 1).type == MaterialType::AIR || container->getParticle(x, y + 1).type == MaterialType::WATER)) {
                Particle temporary_particle = container->getParticle(x, y);
                container->getParticle(x, y) = container->getParticle(x, y + 1);
                container->getParticle(x, y + 1) = temporary_particle;
                if(i + 1 > container->getParticle(x, y + 1).speed_y)
                    container->getParticle(x, y + 1).updated = !even;
                y++;
            }else if((rand() & 1) == 1)
                if(!sandSwapLeftDown(x, y, container, even, i))
                    if(!sandSwapRightDown(x, y, container, even, i)){
                        container->getParticle(x, y).speed_y = 0;
                        break;
                    }
            else
                if(!sandSwapRightDown(x, y, container, even, i))
                    if(!sandSwapLeftDown(x, y, container, even, i)){
                        container->getParticle(x, y).speed_y = 0;
                        break;
                    }

            i++;
        }
        while(i < container->getParticle(x, y).speed_y);
    });

    materials[(int)MaterialType::WATER] = Material({52, 145, 173}, 1.5, [](ParticleContainer* container, int x, int y, bool even){
        container->getParticle(x, y).speed_y *= 0.95;
        container->getParticle(x, y).speed_x *= 0.95;
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
            }else if((rand() & 1) == 1)
                if(!waterSwapLeftDown(x, y, container, even, i))
                    if(!waterSwapRightDown(x, y, container, even, i)){
                        container->getParticle(x, y).speed_y = 0;
                        break;
                    }
            else
                if(!waterSwapRightDown(x, y, container, even, i))
                    if(!waterSwapLeftDown(x, y, container, even, i)){
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
    static int frameCount = 0;
    frameCount++;
    even = !even;
    Particle* iter = getMapBegin();
    for(int i = 0; i < getMapSize(); i++) {
        auto update = iter->getUniqueMaterial().update;
        if(update)
            update(this, i % width, i / width, even);
        iter++;
        if(frameCount < 500 && ((i % width) - 620) * ((i % width) - 500) + ((i / width) - 400) * ((i / width) - 400) < 200 && (rand() & 511) == 0)
            *iter = Particle(MaterialType::SAND);
        if(frameCount < 500 && ((i % width) - 620) * ((i % width) - 500) + ((i / width) - 400) * ((i / width) - 400) < 200 && (rand() & 511) == 0)
            *iter = Particle(MaterialType::WATER);
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

bool sandSwapLeftDown(int x, int y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x, y).updated == even && (container->getParticle(x - 1, y + 1).type == MaterialType::AIR || container->getParticle(x - 1, y + 1).type == MaterialType::WATER)) {
        Particle temporary_particle = container->getParticle(x, y);
        container->getParticle(x, y) = container->getParticle(x - 1, y + 1);
        container->getParticle(x - 1, y + 1) = temporary_particle;
        if(i + 1 > container->getParticle(x, y).speed_y)
            container->getParticle(x - 1, y + 1).updated = !even;
        y++;
        x--;
        return true;
    }
    else
        return false;
}

bool sandSwapRightDown(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x, y).updated == even && (container->getParticle(x + 1, y + 1).type == MaterialType::AIR || container->getParticle(x + 1, y + 1).type == MaterialType::WATER)) {
        Particle temporary_particle = container->getParticle(x, y);
        container->getParticle(x, y) = container->getParticle(x + 1, y + 1);
        container->getParticle(x + 1, y + 1) = temporary_particle;
        if(i + 1 > container->getParticle(x, y).speed_y)
            container->getParticle(x + 1, y + 1).updated = !even;
        y++;
        x++;
        return true;
    }
    else
        return false;
}


bool waterSwapLeftDown(int x, int y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x, y).updated == even && container->getParticle(x - 1, y + 1).type == MaterialType::AIR) {
        Particle temporary_particle = container->getParticle(x, y);
        container->getParticle(x, y) = container->getParticle(x - 1, y + 1);
        container->getParticle(x - 1, y + 1) = temporary_particle;
        if(i + 1 > container->getParticle(x, y).speed_y)
            container->getParticle(x - 1, y + 1).updated = !even;
        y++;
        x--;
        return true;
    }
    else
        return false;
}

bool waterSwapRightDown(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x, y).updated == even && container->getParticle(x + 1, y + 1).type == MaterialType::AIR) {
        Particle temporary_particle = container->getParticle(x, y);
        container->getParticle(x, y) = container->getParticle(x + 1, y + 1);
        container->getParticle(x + 1, y + 1) = temporary_particle;
        if(i + 1 > container->getParticle(x, y).speed_y)
            container->getParticle(x + 1, y + 1).updated = !even;
        y++;
        x++;
        return true;
    }
    else
        return false;
}
