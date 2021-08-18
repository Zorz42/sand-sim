#include "particleContainer.hpp"

void swapParticles(Particle& particle1, Particle& particle2);
bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i);
void lightFire(int x, int y, ParticleContainer* container);
void acidCorrode(int x, int y, ParticleContainer* container);
void fireWaterContact(int x, int y, ParticleContainer* container);
void lavaWaterContact(int x, int y, ParticleContainer* container);
void lightLava(int x, int y, ParticleContainer* container);

Material::Material(std::vector<sf::Color> color, float constant_force, int randomSpawn, void (*update)(ParticleContainer* container, int x, int y, bool even)) : color(color), constant_force(constant_force), randomSpawn(randomSpawn), update(update) {}


void sandUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;

    int i = 0;
    while(i < self->speed_y) {
        Particle& particle_below = container->getParticle(x, y + 1);
        if(self->updated == even) {
            if(particle_below.getMaterial() == &Materials::air || particle_below.getMaterial() == &Materials::water || particle_below.getMaterial() == &Materials::acid || particle_below.getMaterial() == &Materials::americaWantsIt) {
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

    if(self->timer == 1) {
        self->speed_y = 0;
        self->speed_x = 1;
        self->setMaterial(&Materials::air);
        self->timer = 0;
    }

    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != &Materials::air && container->getParticle(x, y + 1).getMaterial() != &Materials::smoke) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != &Materials::air) {
            if(container->getParticle(x + 1, y).getMaterial() == &Materials::air && container->getParticle(x - 1, y).getMaterial() == &Materials::air)
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == &Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == &Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x - 1, y));
                    x--;
                    self = &container->getParticle(x, y);
                }
            }
        }

        self->updated = even;
    }
}

void americaWantsItUpdate(ParticleContainer* container, int x, int y, bool even) {

    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != &Materials::air && container->getParticle(x, y + 1).getMaterial() != &Materials::smoke) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != &Materials::air) {
            if(container->getParticle(x + 1, y).getMaterial() == &Materials::air && container->getParticle(x - 1, y).getMaterial() == &Materials::air)
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == &Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == &Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x - 1, y));
                    x--;
                    self = &container->getParticle(x, y);
                }
            }
        }

        self->updated = even;
    }
}

void lavaUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != &Materials::air && container->getParticle(x, y + 1).getMaterial() != &Materials::smoke && container->getParticle(x, y + 1).getMaterial() != &Materials::fire) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != &Materials::air && container->getParticle(x, y + 1).getMaterial() != &Materials::smoke  && container->getParticle(x, y + 1).getMaterial() != &Materials::fire) {
            if(container->getParticle(x + 1, y).getMaterial() == &Materials::air && container->getParticle(x - 1, y).getMaterial() == &Materials::air &&
               container->getParticle(x + 1, y).getMaterial() == &Materials::smoke && container->getParticle(x - 1, y).getMaterial() == &Materials::smoke &&
               container->getParticle(x + 1, y).getMaterial() == &Materials::fire && container->getParticle(x - 1, y).getMaterial() == &Materials::fire

            )
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == &Materials::air || container->getParticle(x - 1, y).getMaterial() == &Materials::smoke || container->getParticle(x - 1, y).getMaterial() == &Materials::fire)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == &Materials::air || container->getParticle(x - 1, y).getMaterial() == &Materials::smoke || container->getParticle(x - 1, y).getMaterial() == &Materials::fire)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x - 1, y));
                    x--;
                    self = &container->getParticle(x, y);
                }
            }
        }

        self->updated = even;
    }
    lavaWaterContact(x, y, container);
    lightLava(x, y, container);
}

void fireUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    if(self->timer == 0)
        self->timer = 300 + rand() % 150;
    if(self->timer == 1)
        self->setMaterial(&Materials::air);
    self->timer--;

    static int colorChange = 0;
    if(even)
        colorChange++;
    if(colorChange % 16 == 0)
        self->textureColor++;

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

    static int colorChange = 0;
    if(even)
        colorChange++;
    if(colorChange % 64 == 0 && self->textureColor < 6)
        self->textureColor++;

    lightFire(x, y, container);
    fireWaterContact(x, y, container);

    if(std::abs(self->speed_x) < 1 && container->getParticle(x, y - 1).getMaterial() != &Materials::air) {
        if(self->speed_x > 0)
            self->speed_x++;
        else
            self->speed_x--;
    }


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y - 1).getMaterial() != &Materials::air) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y - 1));
                y--;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y - 1).getMaterial() != &Materials::air) {
            if(container->getParticle(x + 1, y).getMaterial() == &Materials::air)
                self->speed_x = (rand() % 2 + 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == &Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == &Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x - 1, y));
                    x--;
                    self = &container->getParticle(x, y);
                }
            }
        }

        self->updated = even;
    }
}

void acidUpdate(ParticleContainer* container, int x, int y, bool even) {
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


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != &Materials::air && container->getParticle(x, y + 1).getMaterial() != &Materials::smoke) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != &Materials::air) {
            if(container->getParticle(x + 1, y).getMaterial() == &Materials::air && container->getParticle(x - 1, y).getMaterial() == &Materials::air)
                self->speed_x = (rand() % 2 + 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == &Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == &Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != &Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x - 1, y));
                    x--;
                    self = &container->getParticle(x, y);
                }
            }
        }

        self->updated = even;
    }
    acidCorrode(x, y, container);
}



void initMaterials() {
    Materials::air = Material({{90, 90, 90}}, 0, 0);
    Materials::sand = Material({{245, 210, 92}, {237, 205, 88}, {239, 207, 90}, {235, 203, 86}, {230, 200, 84}}, 0.08, 80, &sandUpdate);
    Materials::water = Material({{52, 145, 173}, {53, 147, 176}, {47, 140, 169}}, 0.08, 80, &waterUpdate);
    Materials::wood = Material({{155, 116, 54}, {154, 115, 53}, {156, 117, 55}, {150, 111, 51}}, 0, 0);
    Materials::fire = Material({{222, 70, 12}, {222, 120, 16}, {222, 125, 16}, {222, 127, 16}, {222, 130, 16}, {222, 182, 16}}, 0, 20, &fireUpdate);
    Materials::stone = Material({{133, 133, 133}, {135, 135, 135}, {131, 131, 131}}, 0, 0);
    Materials::smoke = Material({{45, 45, 45}, {75, 75, 75}, {105, 105, 105}, {135, 135, 135}, {165, 165, 165}, {195, 195, 195}, {225, 225, 225}}, 0.08, 40, &smokeUpdate);
    Materials::gunpowder = Material({{36, 36, 36}}, 0.08, 40, &sandUpdate);
    Materials::acid = Material({{31, 255, 61}, {32, 255, 62}, {31, 255, 61}}, 0.08, 40, &acidUpdate);
    Materials::americaWantsIt = Material({{10, 10, 10}, {12, 12, 12}, {9, 9, 9}}, 0.08, 40, &americaWantsItUpdate);
    Materials::lava = Material({{252, 91, 16}, {252, 95, 16}, {252, 97, 16}, {252, 102, 16}, {252, 142, 16}}, 0.08, 40, &lavaUpdate);
}

void swapParticles(Particle& particle1, Particle& particle2) {
    Particle temporary_particle = particle1;
    particle1 = particle2;
    particle2 = temporary_particle;
}

bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i) {
    Particle& target_particle = container->getParticle(target_x, target_y);
    Particle& self = container->getParticle(x, y);
    if(self.updated == even && (target_particle.getMaterial() == &Materials::air || target_particle.getMaterial() == &Materials::water || target_particle.getMaterial() == &Materials::acid || target_particle.getMaterial() == &Materials::americaWantsIt)) {
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
        else if(particle->getMaterial() == &Materials::gunpowder && rand() % 2 == 0){
            particle->setMaterial(&Materials::fire);
            particle->timer = 20 - rand() % 5;
        } else if(particle->getMaterial() == &Materials::americaWantsIt && rand() % 8 == 0){
            particle->setMaterial(&Materials::fire);
            particle->timer = 60 - rand() % 10;
        }
}


void acidCorrode(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y), &container->getParticle(x, y - 1)};
    for(Particle* particle : particles)
        if(particle->getMaterial() == &Materials::wood && rand() % 20 == 0){
            container->getParticle(x, y).setMaterial(&Materials::air);
            if(rand() % 10 == 0)
                particle->setMaterial(&Materials::air);
            else
                particle->setMaterial(&Materials::acid);
        }else if(particle->getMaterial() == &Materials::water && rand() % 3 == 0)
            particle->setMaterial(&Materials::acid);

}

void fireWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->getMaterial() == &Materials::water){
            particle->setMaterial(&Materials::smoke);
            self->setMaterial(&Materials::smoke);
            self->textureColor = 0;
            particle->textureColor = 0;
        }else if(particle->getMaterial() == &Materials::acid){
            self->setMaterial(&Materials::smoke);
            self->timer = 60;
            self->textureColor = 0;
            particle->setMaterial(&Materials::air);
        }
}

void lavaWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->getMaterial() == &Materials::water){
            container->getParticle(x, y - 2).setMaterial(&Materials::smoke);
            particle->setMaterial(&Materials::stone);
            self->setMaterial(&Materials::stone);
            particle->textureColor = 0;
        }else if(particle->getMaterial() == &Materials::acid){
            self->setMaterial(&Materials::stone);
            particle->setMaterial(&Materials::air);
        }
}

void lightLava(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};
    for(Particle* particle : particles)
        if(particle->getMaterial() == &Materials::wood && rand() % 50 == 0)
            particle->setMaterial(&Materials::fire);
        else if(particle->getMaterial() == &Materials::gunpowder){
            particle->setMaterial(&Materials::fire);
            particle->timer = 20 - rand() % 5;
        } else if(particle->getMaterial() == &Materials::americaWantsIt && rand() % 2 == 0){
            particle->setMaterial(&Materials::fire);
            particle->timer = 60 - rand() % 10;
        }
}
