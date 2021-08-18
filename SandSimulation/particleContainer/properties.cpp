#include "particleContainer.hpp"

void swapParticles(Particle& particle1, Particle& particle2);
bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i);
void lightFire(int x, int y, ParticleContainer* container);
void acidCorrode(int x, int y, ParticleContainer* container);
void fireWaterContact(int x, int y, ParticleContainer* container);
void lavaWaterContact(int x, int y, ParticleContainer* container);
void lightLava(int x, int y, ParticleContainer* container);

Material::Material(std::vector<sf::Color> color, float constant_force, int random_spawn)
: color(color), constant_force(constant_force), random_spawn(random_spawn) {}


void Sand::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;

    int i = 0;
    while(i < self->speed_y) {
        Particle& particle_below = container->getParticle(x, y + 1);
        if(self->updated == even) {
            if(particle_below.getMaterial() == Materials::air || particle_below.getMaterial() == Materials::water || particle_below.getMaterial() == Materials::acid || particle_below.getMaterial() == Materials::oil) {
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

void Gunpowder::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;

    int i = 0;
    while(i < self->speed_y) {
        Particle& particle_below = container->getParticle(x, y + 1);
        if(self->updated == even) {
            if(particle_below.getMaterial() == Materials::air || particle_below.getMaterial() == Materials::water || particle_below.getMaterial() == Materials::acid || particle_below.getMaterial() == Materials::oil) {
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

void Water::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;
    if(self->timer == 0)
        self->timer = 3600;

    if(container->getParticle(x, y - 1).getMaterial() == Materials::air)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1) {
        self->speed_y = 0;
        self->speed_x = 1;
        self->setMaterial(Materials::air);
        self->timer = 0;
    }

    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != Materials::air &&
            container->getParticle(x, y + 1).getMaterial() != Materials::smoke &&
            container->getParticle(x, y + 1).getMaterial() != Materials::oil) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != Materials::air &&
        container->getParticle(x, y + 1).getMaterial() != Materials::oil &&
        container->getParticle(x, y + 1).getMaterial() != Materials::smoke) {
            if((container->getParticle(x + 1, y).getMaterial() == Materials::air || container->getParticle(x + 1, y).getMaterial() == Materials::oil || container->getParticle(x + 1, y).getMaterial() == Materials::smoke) &&
            (container->getParticle(x - 1, y).getMaterial() == Materials::air || container->getParticle(x - 1, y).getMaterial() == Materials::oil || container->getParticle(x - 1, y).getMaterial() == Materials::smoke))
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == Materials::air || container->getParticle(x + 1, y).getMaterial() == Materials::smoke || container->getParticle(x + 1, y).getMaterial() == Materials::oil)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == Materials::air || container->getParticle(x + 1, y).getMaterial() == Materials::smoke || container->getParticle(x + 1, y).getMaterial() == Materials::oil)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != Materials::air && container->getParticle(x + 1, y).getMaterial() != Materials::smoke && container->getParticle(x + 1, y).getMaterial() != Materials::oil)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != Materials::air && container->getParticle(x + 1, y).getMaterial() != Materials::smoke && container->getParticle(x + 1, y).getMaterial() != Materials::oil)
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

void Oil::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != Materials::air && container->getParticle(x, y + 1).getMaterial() != Materials::smoke) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != Materials::air) {
            if(container->getParticle(x + 1, y).getMaterial() == Materials::air && container->getParticle(x - 1, y).getMaterial() == Materials::air)
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != Materials::air)
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

void Lava::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != Materials::air && container->getParticle(x, y + 1).getMaterial() != Materials::smoke && container->getParticle(x, y + 1).getMaterial() != Materials::fire) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != Materials::air && container->getParticle(x, y + 1).getMaterial() != Materials::smoke  && container->getParticle(x, y + 1).getMaterial() != Materials::fire) {
            if(container->getParticle(x + 1, y).getMaterial() == Materials::air && container->getParticle(x - 1, y).getMaterial() == Materials::air &&
               container->getParticle(x + 1, y).getMaterial() == Materials::smoke && container->getParticle(x - 1, y).getMaterial() == Materials::smoke &&
               container->getParticle(x + 1, y).getMaterial() == Materials::fire && container->getParticle(x - 1, y).getMaterial() == Materials::fire

            )
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == Materials::air || container->getParticle(x - 1, y).getMaterial() == Materials::smoke || container->getParticle(x - 1, y).getMaterial() == Materials::fire)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == Materials::air || container->getParticle(x - 1, y).getMaterial() == Materials::smoke || container->getParticle(x - 1, y).getMaterial() == Materials::fire)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != Materials::air)
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

void Fire::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    if(self->timer == 0)
        self->timer = 300 + rand() % 150;
    if(self->timer == 1)
        self->setMaterial(Materials::air);
    self->timer--;

    static int colorChange = 0;
    if(even)
        colorChange++;
    if(colorChange % 16 == 0)
        self->texture_color++;

    lightFire(x, y, container);
    fireWaterContact(x, y, container);
}

void Smoke::update(ParticleContainer* container, int x, int y, bool even){
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.998;
    self->speed_x *= 0.998;
    self->speed_y += self->getMaterial()->constant_force;
    if(self->timer == 0)
        self->timer = 600;

    if(container->getParticle(x, y + 1).getMaterial() == Materials::air)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1) {
        self->speed_y = 0;
        self->speed_x = 1;
        self->setMaterial(Materials::air);
        self->timer = 0;
    }

    static int colorChange = 0;
    if(even)
        colorChange++;
    if(colorChange % 64 == 0 && self->texture_color < 6)
        self->texture_color++;

    lightFire(x, y, container);
    fireWaterContact(x, y, container);

    if(std::abs(self->speed_x) < 1 && container->getParticle(x, y - 1).getMaterial() != Materials::air) {
        if(self->speed_x > 0)
            self->speed_x++;
        else
            self->speed_x--;
    }


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y - 1).getMaterial() != Materials::air) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y - 1));
                y--;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y - 1).getMaterial() != Materials::air) {
            if(container->getParticle(x + 1, y).getMaterial() == Materials::air)
                self->speed_x = (rand() % 2 + 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != Materials::air)
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

void Acid::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getMaterial()->constant_force;
    if(self->timer == 0)
        self->timer = 3600;

    if(container->getParticle(x, y - 1).getMaterial() == Materials::air)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1){
        self->speed_y = 0;
        self->speed_x = 1;
        self->setMaterial(Materials::air);
        self->timer = 0;
    }

    if(std::abs(self->speed_x) < 1 && container->getParticle(x, y + 1).getMaterial() != Materials::air) {
        if(self->speed_x > 0)
            self->speed_x++;
        else
            self->speed_x--;
    }


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).getMaterial() != Materials::air && container->getParticle(x, y + 1).getMaterial() != Materials::smoke) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).getMaterial() != Materials::air) {
            if(container->getParticle(x + 1, y).getMaterial() == Materials::air && container->getParticle(x - 1, y).getMaterial() == Materials::air)
                self->speed_x = (rand() % 2 + 1) * 5;
            else if(container->getParticle(x + 1, y).getMaterial() == Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).getMaterial() == Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).getMaterial() != Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).getMaterial() != Materials::air)
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
    Materials::air = new Air();
    Materials::sand = new Sand();
    Materials::water = new Water();
    Materials::wood = new Wood();
    Materials::fire = new Fire();
    Materials::stone = new Stone();
    Materials::smoke = new Smoke();
    Materials::gunpowder = new Gunpowder();
    Materials::acid = new Acid();
    Materials::oil = new Oil();
    Materials::lava = new Lava();
}

void swapParticles(Particle& particle1, Particle& particle2) {
    Particle temporary_particle = particle1;
    particle1 = particle2;
    particle2 = temporary_particle;
}

bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i) {
    Particle& target_particle = container->getParticle(target_x, target_y);
    Particle& self = container->getParticle(x, y);
    if(self.updated == even && (target_particle.getMaterial() == Materials::air || target_particle.getMaterial() == Materials::water || target_particle.getMaterial() == Materials::acid || target_particle.getMaterial() == Materials::oil)) {
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
        if(particle->getMaterial() == Materials::wood && rand() % 200 == 0)
            particle->setMaterial(Materials::fire);
        else if(particle->getMaterial() == Materials::gunpowder && rand() % 2 == 0){
            particle->setMaterial(Materials::fire);
            particle->timer = 20 - rand() % 5;
        } else if(particle->getMaterial() == Materials::oil && rand() % 8 == 0){
            particle->setMaterial(Materials::fire);
            particle->timer = 60 - rand() % 10;
        }
}


void acidCorrode(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y), &container->getParticle(x, y - 1)};
    for(Particle* particle : particles)
        if(particle->getMaterial() == Materials::wood && rand() % 20 == 0){
            container->getParticle(x, y).setMaterial(Materials::air);
            if(rand() % 10 == 0)
                particle->setMaterial(Materials::air);
            else
                particle->setMaterial(Materials::acid);
        }else if(particle->getMaterial() == Materials::water && rand() % 3 == 0)
            particle->setMaterial(Materials::acid);

}

void fireWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->getMaterial() == Materials::water){
            particle->setMaterial(Materials::smoke);
            self->setMaterial(Materials::smoke);
            self->texture_color = 0;
            particle->texture_color = 0;
        }else if(particle->getMaterial() == Materials::acid){
            self->setMaterial(Materials::smoke);
            self->timer = 60;
            self->texture_color = 0;
            particle->setMaterial(Materials::air);
        }
}

void lavaWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->getMaterial() == Materials::water){
            container->getParticle(x, y - 2).setMaterial(Materials::smoke);
            particle->setMaterial(Materials::stone);
            self->setMaterial(Materials::stone);
            particle->texture_color = 0;
        }else if(particle->getMaterial() == Materials::acid){
            self->setMaterial(Materials::stone);
            particle->setMaterial(Materials::air);
        }
}

void lightLava(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};
    for(Particle* particle : particles)
        if(particle->getMaterial() == Materials::wood && rand() % 50 == 0)
            particle->setMaterial(Materials::fire);
        else if(particle->getMaterial() == Materials::gunpowder){
            particle->setMaterial(Materials::fire);
            particle->timer = 20 - rand() % 5;
        } else if(particle->getMaterial() == Materials::oil && rand() % 2 == 0){
            particle->setMaterial(Materials::fire);
            particle->timer = 60 - rand() % 10;
        }
}
