#include "particleContainer.hpp"

void swapParticles(Particle& particle1, Particle& particle2);
bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i);
void lightFire(int x, int y, ParticleContainer* container);
void fireWaterContact(int x, int y, ParticleContainer* container);

Material::Material(sf::Color color, float constant_force, int randomSpawn, void (*update)(ParticleContainer* container, int x, int y, bool even)) : color(color), constant_force(constant_force), randomSpawn(randomSpawn), update(update) {}


void sandUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;

    int i = 0;
    while(i < self->speed_y) {
        Particle& particle_below = container->getParticle(x, y + 1);
        if(self->updated == even) {
            if(particle_below.getMaterial() == &Materials::air || particle_below.getMaterial() == &Materials::water) {
                swapParticles(*self, particle_below);
                if(i + 1 > particle_below.speed_y)
                    particle_below.updated = !even;
                y++;
                self = &container->getParticle(x, y);
            } else if(rand() % 2 == 1) {
                if(!sandSwap(container, x, y, x - 1, y + 1, even, i))
                    if(!sandSwap(container, x, y, x + 1, y + 1, even, i)) {
                        self->speed_y = 0;
                        break;
                    }
            } else if(!sandSwap(container, x, y, x + 1, y + 1, even, i))
                if(!sandSwap(container, x, y, x - 1, y + 1, even, i)) {
                    self->speed_y = 0;
                    break;
            }
        }
        i++;
    }
}

void waterUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;
    if(self->timer == 0)
        self->timer = 3600;

    if(container->getParticle(x, y - 1).getMaterial() == &Materials::air)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1){
        self->speed_y = 0;
        self->speed_x = 1;
        self->setMaterial(&Materials::air);
        self->timer = 0;
    }

    if(std::abs(self->speed_x) < 1 && container->getParticle(x, y + 1).getMaterial() != &Materials::air) {
        if(self->speed_x > 0)
            self->speed_x++;
        else
            self->speed_x--;
    }


    if(self->updated != even){
        float speedXCopy = self->speed_x, speedYCopy = self->speed_y;
        int prevX, prevY;
        do{
            prevX = x;
            prevY = y;
            if(speedYCopy > 0 && container->getParticle(x, y + 1).getMaterial() == &Materials::air || container->getParticle(x, y + 1).getMaterial() == &Materials::smoke) {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                speedYCopy -= 1;
                self = &container->getParticle(x, y);
            } else if(container->getParticle(x, y + 1).getMaterial() != &Materials::air && container->getParticle(x, y + 1).getMaterial() != &Materials::smoke) {
                if(speedYCopy > 0 && container->getParticle(x - 1, y + 1).getMaterial() == &Materials::air || container->getParticle(x - 1, y + 1).getMaterial() == &Materials::smoke) {
                    swapParticles(*self, container->getParticle(x - 1, y + 1));
                    y++;
                    x--;
                    speedYCopy -= 1;
                    speedXCopy += 1;
                    self->speed_x = std::max((float)-1, self->speed_x - 1);
                    self = &container->getParticle(x, y);
                } else if(speedYCopy > 0 && container->getParticle(x + 1, y + 1).getMaterial() == &Materials::air || container->getParticle(x + 1, y + 1).getMaterial() == &Materials::smoke) {
                    swapParticles(*self, container->getParticle(x + 1, y + 1));
                    y++;
                    x++;
                    speedYCopy -= 1;
                    speedXCopy -= 1;
                    self->speed_x = std::max((float)1, self->speed_x + 1);
                    self = &container->getParticle(x, y);
                } else if(speedXCopy >= 1 && container->getParticle(x + 1, y).getMaterial() == &Materials::air) {
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    speedXCopy -= 1;
                    self->speed_y = 0;
                    self = &container->getParticle(x, y);
                } else if(speedXCopy <= -1 && container->getParticle(x - 1, y).getMaterial() == &Materials::air) {
                    swapParticles(*self, container->getParticle(x - 1, y));
                    x--;
                    speedXCopy += 1;
                    self->speed_y = 0;
                    self = &container->getParticle(x, y);
                }  else{if(speedXCopy >= 1)
                    self->speed_x = -1;
                    else if(speedXCopy <= -1)
                        self->speed_x = 1;
                    if(speedYCopy > 0)
                        self->speed_y = 0;
                }
            }
        } while(prevY != y || prevX != x);
        self->updated = even;
    }






    /*
    bool moved = false;
    int i = 0;
    if(self->updated == even){
        while(i < self->speed_y && self->updated == even) {
            Particle& particle_below = container->getParticle(x, y + 1);
            if(particle_below.getType() == &Materials::air || particle_below.getType() == &Materials::smoke) {
                swapParticles(*self, particle_below);
                moved = true;
                if(i + 1 > particle_below.speed_y)
                    particle_below.updated = !even;
                y++;
                self = &container->getParticle(x, y);

            }else if(!waterSwapDiagonal(container, x, y, x - 1, y + 1, even, i)){
                if(!waterSwapDiagonal(container, x, y, x + 1, y + 1, even, i)) {
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

        if(!moved){
            i = 0;
            while(i < self->speed_x) {
                if(!waterSwapHorizontal(container, x, y, x + 1, y, even, i))
                    self->speed_x = -1;
                i++;
            }
            i = 0;
            while(i < -1 * self->speed_x) {
                if(!waterSwapHorizontal(container, x, y, x - 1, y, even, i))
                    self->speed_x = 1;
                i++;
            }
        }
    }*/
}

void fireUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    if(self->timer == 0)
        self->timer = 300 + rand() % 150;
    if(self->timer == 1)
        self->setMaterial(&Materials::air);
    self->timer--;

    lightFire(x, y, container);
    fireWaterContact(x, y, container);
}

void smokeUpdate(ParticleContainer* container, int x, int y, bool even){
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.998;
    self->speed_x *= 0.998;
    self->speed_y += self->getMaterial()->constant_force;
    if(self->timer == 0)
        self->timer = 600;

    if(container->getParticle(x, y + 1).getMaterial() == &Materials::air)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1) {
        self->speed_y = 0;
        self->speed_x = 1;
        self->setMaterial(&Materials::air);
        self->timer = 0;
    }

    if(std::abs(self->speed_x) < 1 && container->getParticle(x, y - 1).getMaterial() != &Materials::air) {
        if(self->speed_x > 0)
            self->speed_x++;
        else
            self->speed_x--;
    }


    if(self->updated != even){
        float speedXCopy = self->speed_x, speedYCopy = self->speed_y;
        int prevX, prevY;
        do{
            prevX = x;
            prevY = y;
            if(speedYCopy > 0 && container->getParticle(x, y - 1).getMaterial() == &Materials::air) {
                swapParticles(*self, container->getParticle(x, y - 1));
                y--;
                speedYCopy -= 1;
                self = &container->getParticle(x, y);
            } else if(container->getParticle(x, y - 1).getMaterial() != &Materials::air) {
                if(speedYCopy > 0 && container->getParticle(x - 1, y - 1).getMaterial() == &Materials::air) {
                    swapParticles(*self, container->getParticle(x - 1, y - 1));
                    y--;
                    x--;
                    speedYCopy -= 1;
                    speedXCopy += 1;
                    self->speed_x = std::max((float)-1, self->speed_x - 1);
                    self = &container->getParticle(x, y);
                } else if(speedYCopy > 0 && container->getParticle(x + 1, y - 1).getMaterial() == &Materials::air) {
                    swapParticles(*self, container->getParticle(x + 1, y - 1));
                    y--;
                    x++;
                    speedYCopy -= 1;
                    speedXCopy -= 1;
                    self->speed_x = std::max((float)1, self->speed_x + 1);
                    self = &container->getParticle(x, y);
                } else if(speedXCopy >= 1 && container->getParticle(x + 1, y).getMaterial() == &Materials::air) {
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    speedXCopy -= 1;
                    self->speed_y = 0;
                    self = &container->getParticle(x, y);
                } else if(speedXCopy <= -1 && container->getParticle(x - 1, y).getMaterial() == &Materials::air) {
                    swapParticles(*self, container->getParticle(x - 1, y));
                    x--;
                    speedXCopy += 1;
                    self->speed_y = 0;
                    self = &container->getParticle(x, y);
                }  else{if(speedXCopy >= 1)
                    self->speed_x = -1;
                    else if(speedXCopy <= -1)
                        self->speed_x = 1;
                    if(speedYCopy > 0)
                        self->speed_y = 0;
                }
            }
        } while(prevY != y || prevX != x);
        self->updated = even;
    }
}


void initMaterials() {
    Materials::air = Material({90, 90, 90}, 0, 0);
    Materials::sand = Material({237, 205, 88}, 0.08, 80, &sandUpdate);
    Materials::water = Material({52, 145, 173}, 0.08, 80, &waterUpdate);
    Materials::wood = Material({150, 111, 51}, 0, 0);
    Materials::fire = Material({222, 91, 16}, 0, 20, &fireUpdate);
    Materials::stone = Material({133, 133, 133}, 0, 0);
    Materials::smoke = Material({45, 45, 45}, 0.08, 40, &smokeUpdate);
}

void swapParticles(Particle& particle1, Particle& particle2) {
    Particle temporary_particle = particle1;
    particle1 = particle2;
    particle2 = temporary_particle;
}

bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i) {
    Particle& target_particle = container->getParticle(target_x, target_y);
    Particle& self = container->getParticle(x, y);
    if(self.updated == even && (target_particle.getMaterial() == &Materials::air || target_particle.getMaterial() == &Materials::water)) {
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

void lightFire(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};
    for(Particle* particle : particles)
        if(particle->getMaterial() == &Materials::wood && rand() % 200 == 0)
            particle->setMaterial(&Materials::fire);
}

void fireWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->getMaterial() == &Materials::water){
            particle->setMaterial(&Materials::smoke);
            self->setMaterial(&Materials::smoke);
        }
}
