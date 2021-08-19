#include "particleEngine.hpp"

ParticleContainer::ParticleContainer(int size_x, int size_y) : width(size_x), height(size_y) {
    map = new Particle[size_x * size_y];
}

bool powderSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i);

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
    if(material->type != MaterialType::SOLID) {
        speed_y *= 0.995;
        speed_x *= 0.995;
        speed_y += 0.1;
    }
    
    if(material->type == MaterialType::POWDER) {
        Particle* self = &container->getParticle(x, y);

        int i = 0;
        while(i < self->speed_y) {
            Particle& particle_below = container->getParticle(x, y + 1);
            if(self->updated == even) {
                if(particle_below.material == Materials::air || particle_below.material->type == MaterialType::LIQUID) {
                    swapParticles(*self, particle_below);
                    if(i + 1 > particle_below.speed_y)
                        particle_below.updated = !even;
                    y++;
                    self = &container->getParticle(x, y);
                } else if(rand() % 2 == 1) {
                    if(!powderSwap(container, x, y, x - 1, y + 1, even, i))
                        if(!powderSwap(container, x, y, x + 1, y + 1, even, i)) {
                            self->speed_y = 0;
                            break;
                        }
                } else if(!powderSwap(container, x, y, x + 1, y + 1, even, i))
                    if(!powderSwap(container, x, y, x - 1, y + 1, even, i)) {
                        self->speed_y = 0;
                        break;
                }
            }
            i++;
        }
    }
    material->update(container, x, y, even);
}

Air::Air() {
    color = {{91, 91, 91}};
    type = MaterialType::SOLID;
}

Stone::Stone() {
    color = {{133, 133, 133}, {135, 135, 135}, {131, 131, 131}};
    type = MaterialType::SOLID;
}

void swapParticles(Particle& particle1, Particle& particle2) {
    Particle temporary_particle = particle1;
    particle1 = particle2;
    particle2 = temporary_particle;
}

bool powderSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i) {
    Particle& target_particle = container->getParticle(target_x, target_y);
    Particle& self = container->getParticle(x, y);
    if(self.updated == even && (target_particle.material == Materials::air || target_particle.material->type == MaterialType::LIQUID)) {
        swapParticles(self, target_particle);
        if(i + 1 > self.speed_y)
            target_particle.updated = !even;
        y = target_y;
        x = target_x;
        return true;
    }
    else
        return false;
}
