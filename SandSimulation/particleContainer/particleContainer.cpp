#include <vector>
#include "particleContainer.hpp"

void swapParticles(Particle& particle1, Particle& particle2);

Material materials[(int)MaterialType::NUM_MATERIALS];

void initMaterials() {
    materials[(int)MaterialType::AIR] = Material({90, 90, 90}, 0, 1);

    materials[(int)MaterialType::SAND] = Material({237, 205, 88}, 0.08, 80, [](ParticleContainer* container, int x, int y, bool even) {
        Particle* self = &container->getParticle(x, y);
        self->speed_y *= 0.995;
        self->speed_x *= 0.995;
        self->speed_y += self->getUniqueMaterial().constant_force;

        int i = 0;
        while(i < self->speed_y) {
            Particle& particle_below = container->getParticle(x, y + 1);
            if(self->updated == even) {
                if(particle_below.type == MaterialType::AIR || particle_below.type == MaterialType::WATER) {
                    swapParticles(*self, particle_below);
                    if(i + 1 > particle_below.speed_y)
                        particle_below.updated = !even;
                    y++;
                    self = &container->getParticle(x, y);
                } else if(rand() % 2 == 1) {
                    if(!sandSwapLeftDown(x, y, container, even, i))
                        if(!sandSwapRightDown(x, y, container, even, i)) {
                            self->speed_y = 0;
                            break;
                        }
                } else if(!sandSwapRightDown(x, y, container, even, i))
                    if(!sandSwapLeftDown(x, y, container, even, i)) {
                        self->speed_y = 0;
                        break;
                }
            }
            i++;
        }
    });

    materials[(int)MaterialType::WATER] = Material({52, 145, 173}, 0.05, 80, [](ParticleContainer* container, int x, int y, bool even) {
        Particle* self = &container->getParticle(x, y);
        self->speed_y *= 0.995;
        self->speed_x *= 0.995;
        self->speed_y += self->getUniqueMaterial().constant_force;

        bool moved = false;
        int i = 0;
        if(self->updated == even){
            while(i < self->speed_y && self->updated == even) {
                Particle& particle_below = container->getParticle(x, y + 1);
                if(particle_below.type == MaterialType::AIR) {
                    swapParticles(*self, particle_below);
                    moved = true;
                    if(i + 1 > particle_below.speed_y)
                        particle_below.updated = !even;
                    y++;
                    self = &container->getParticle(x, y);

                }else if(!waterSwapLeftDown(x, y, container, even, i)){
                    if(!waterSwapRightDown(x, y, container, even, i)) {
                        self->speed_y = 0;
                    } else{
                        self->speed_x = std::max((float)1, self->speed_x + 1);
                        moved = true;
                    }
                } else{
                    self->speed_x = std::min((float)-1, self->speed_x - 1);
                    moved = true;
                }


                i++;
            }

            if(self->speed_y != 0)
                self->speed_x += self->getUniqueMaterial().constant_force / 3;

            if(!moved){
                i = 0;
                while(i < self->speed_x) {
                    if(!waterSwapLeft(x, y, container, even, i))
                        self->speed_x = -1;
                    i++;
                }
                i = 0;
                while(i < -1 * self->speed_x) {
                    if(!waterSwapRight(x, y, container, even, i))
                        self->speed_x = 1;
                    i++;
                }
            }
        }



    });

    materials[(int)MaterialType::WOOD] = Material({105, 73, 6}, 0, 1/*, [](ParticleContainer* container, int x, int y, bool even){

    }*/);
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
    }
}

Particle& ParticleContainer::getParticle(unsigned short x, unsigned short y) {
    static Particle out_of_bounds;
    if(x < 0 || x >= width || y < 0 || y >= height)
        return out_of_bounds;
    return map[y * width + x];
}

const Material& getMaterialByType(MaterialType type) {
    return materials[(int)type];
}

const Material& Particle::getUniqueMaterial() {
    return getMaterialByType(type);
}

unsigned int ParticleContainer::getMapSize() {
    return width * height;
}

void swapParticles(Particle& particle1, Particle& particle2) {
    Particle temporary_particle = particle1;
    particle1 = particle2;
    particle2 = temporary_particle;
}

bool sandSwapLeftDown(int x, int y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x, y).updated == even && (container->getParticle(x - 1, y + 1).type == MaterialType::AIR || container->getParticle(x - 1, y + 1).type == MaterialType::WATER)) {
        swapParticles(container->getParticle(x, y), container->getParticle(x - 1, y + 1));
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
        swapParticles(container->getParticle(x, y), container->getParticle(x + 1, y + 1));
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
    if(container->getParticle(x - 1, y + 1).type == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x - 1, y + 1));
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
    if(container->getParticle(x + 1, y + 1).type == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x + 1, y + 1));
        if(i + 1 > container->getParticle(x, y).speed_y)
            container->getParticle(x + 1, y + 1).updated = !even;
        y++;
        x++;
        return true;
    }
    else
        return false;
}

bool waterSwapLeft(int x, int y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x - 1, y).type == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x - 1, y));
        if(i + 1 > container->getParticle(x, y).speed_x)
            container->getParticle(x - 1, y).updated = !even;
        x--;
        return true;
    }
    else
        return false;
}

bool waterSwapRight(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x + 1, y).type == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x + 1, y));
        if(i + 1 > container->getParticle(x, y).speed_x)
            container->getParticle(x + 1, y).updated = !even;
        x++;
        return true;
    }
    else
        return false;
}


