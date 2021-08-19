#include "particles.hpp"

void swapParticles(Particle& particle1, Particle& particle2);
bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i);
void lightFire(int x, int y, ParticleContainer* container);
void acidCorrode(int x, int y, ParticleContainer* container);
void fireWaterContact(int x, int y, ParticleContainer* container);
void lavaWaterContact(int x, int y, ParticleContainer* container);
void lightLava(int x, int y, ParticleContainer* container);


Sand::Sand() {
    color = {{245, 210, 92}, {237, 205, 88}, {239, 207, 90}, {235, 203, 86}, {231, 200, 84}};
    random_spawn = 80;
}

void Sand::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->material->constant_force;

    int i = 0;
    while(i < self->speed_y) {
        Particle& particle_below = container->getParticle(x, y + 1);
        if(self->updated == even) {
            if(particle_below.material == Materials::air || particle_below.material == Materials::water || particle_below.material == Materials::acid || particle_below.material == Materials::oil) {
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

Wood::Wood() {
    color = {{155, 116, 54}, {153, 115, 53}, {157, 117, 55}, {151, 111, 51}};
    constant_force = 0;
}

Gunpowder::Gunpowder() {
    color = {{37, 37, 37}};
    random_spawn = 40;
}

void Gunpowder::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->material->constant_force;

    int i = 0;
    while(i < self->speed_y) {
        Particle& particle_below = container->getParticle(x, y + 1);
        if(self->updated == even) {
            if(particle_below.material == Materials::air || particle_below.material == Materials::water || particle_below.material == Materials::acid || particle_below.material == Materials::oil) {
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

Water::Water() {
    color = {{53, 145, 173}, {53, 147, 176}, {47, 140, 169}};
    random_spawn = 80;
}

void Water::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->material->constant_force;
    if(self->timer == 0)
        self->timer = 3600;

    if(container->getParticle(x, y - 1).material == Materials::air)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1) {
        self->speed_y = 0;
        self->speed_x = 1;
        self->material = Materials::air;
        self->timer = 0;
    }

    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).material != Materials::air &&
            container->getParticle(x, y + 1).material != Materials::smoke &&
            container->getParticle(x, y + 1).material != Materials::oil) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).material != Materials::air &&
        container->getParticle(x, y + 1).material != Materials::oil &&
        container->getParticle(x, y + 1).material != Materials::smoke) {
            if((container->getParticle(x + 1, y).material == Materials::air || container->getParticle(x + 1, y).material == Materials::oil || container->getParticle(x + 1, y).material == Materials::smoke) &&
            (container->getParticle(x - 1, y).material == Materials::air || container->getParticle(x - 1, y).material == Materials::oil || container->getParticle(x - 1, y).material == Materials::smoke))
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).material == Materials::air || container->getParticle(x + 1, y).material == Materials::smoke || container->getParticle(x + 1, y).material == Materials::oil)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).material == Materials::air || container->getParticle(x + 1, y).material == Materials::smoke || container->getParticle(x + 1, y).material == Materials::oil)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).material != Materials::air && container->getParticle(x + 1, y).material != Materials::smoke && container->getParticle(x + 1, y).material != Materials::oil)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).material != Materials::air && container->getParticle(x + 1, y).material != Materials::smoke && container->getParticle(x + 1, y).material != Materials::oil)
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

Oil::Oil() {
    color = {{11, 11, 11}, {11, 12, 12}, {9, 9, 9}};
    random_spawn = 40;
}

void Oil::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->material->constant_force;


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).material != Materials::air && container->getParticle(x, y + 1).material != Materials::smoke) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).material != Materials::air) {
            if(container->getParticle(x + 1, y).material == Materials::air && container->getParticle(x - 1, y).material == Materials::air)
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).material == Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).material == Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).material != Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).material != Materials::air)
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

Lava::Lava() {
    color = {{252, 91, 16}, {252, 95, 16}, {252, 97, 16}, {252, 102, 16}, {252, 142, 16}};
    random_spawn = 40;
}

void Lava::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->material->constant_force;


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).material != Materials::air && container->getParticle(x, y + 1).material != Materials::smoke && container->getParticle(x, y + 1).material != Materials::fire) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).material != Materials::air && container->getParticle(x, y + 1).material != Materials::smoke  && container->getParticle(x, y + 1).material != Materials::fire) {
            if(container->getParticle(x + 1, y).material == Materials::air && container->getParticle(x - 1, y).material == Materials::air &&
               container->getParticle(x + 1, y).material == Materials::smoke && container->getParticle(x - 1, y).material == Materials::smoke &&
               container->getParticle(x + 1, y).material == Materials::fire && container->getParticle(x - 1, y).material == Materials::fire

            )
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if((container->getParticle(x + 1, y).material == Materials::air || container->getParticle(x - 1, y).material == Materials::smoke || container->getParticle(x - 1, y).material == Materials::fire))
                self->speed_x = 5;
            else if((container->getParticle(x - 1, y).material == Materials::air || container->getParticle(x - 1, y).material == Materials::smoke || container->getParticle(x - 1, y).material == Materials::fire))
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).material != Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).material != Materials::air)
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

Fire::Fire() {
    color = {{222, 70, 12}, {222, 120, 16}, {222, 125, 16}, {222, 127, 16}, {222, 130, 16}, {222, 182, 16}};
    constant_force = 0;
}

void Fire::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    if(self->timer == 0)
        self->timer = 300 + rand() % 150;
    if(self->timer == 1)
        self->material = Materials::air;
    self->timer--;

    static int colorChange = 0;
    colorChange++;
    if(colorChange % 32 == 0)
        self->texture_color++;

    lightFire(x, y, container);
    fireWaterContact(x, y, container);
    if(container->getParticle(x, y - 1).material == Materials::air && rand() % 10 == 0)
        container->getParticle(x, y - 1).material = Materials::smoke;
}

Smoke::Smoke() {
    color = {{65, 65, 59}, {61, 60, 54}, {69, 70, 64}};
    random_spawn = 40;
}

void Smoke::update(ParticleContainer* container, int x, int y, bool even){
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.998;
    self->speed_x *= 0.998;
    self->speed_y += self->material->constant_force;
    if(self->timer == 0)
        self->timer = 600;

    if(y == 0 && self->timer > 30)
        self->timer = 30;

    if(container->getParticle(x, y + 1).material == Materials::air || y == 0)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1) {
        self->speed_y = 0;
        self->speed_x = 1;
        self->material = Materials::air;
        self->timer = 0;
    }

    static int colorChange = 0;
    if(even)
        colorChange++;
    if(colorChange % 64 == 0)
        self->texture_color++;

    if(std::abs(self->speed_x) < 1 && container->getParticle(x, y - 1).material != Materials::air) {
        if(self->speed_x > 0)
            self->speed_x++;
        else
            self->speed_x--;
    }


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y - 1).material != Materials::air) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y - 1));
                y--;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y - 1).material != Materials::air) {
            if(container->getParticle(x + 1, y).material == Materials::air)
                self->speed_x = (rand() % 2 + 1) * 5;
            else if(container->getParticle(x + 1, y).material == Materials::air)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).material == Materials::air)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).material != Materials::air)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).material != Materials::air)
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

Acid::Acid() {
    color = {{30, 255, 61}, {32, 255, 62}, {32, 255, 61}};
    random_spawn = 40;
}

void Acid::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->material->constant_force;
    if(self->timer == 0)
        self->timer = 3600;

    if(container->getParticle(x, y - 1).material == Materials::air)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1){
        self->speed_y = 0;
        self->speed_x = 1;
        self->material = Materials::air;
        self->timer = 0;
    }

    if(std::abs(self->speed_x) < 1 && container->getParticle(x, y + 1).material != Materials::air) {
        if(self->speed_x > 0)
            self->speed_x++;
        else
            self->speed_x--;
    }


    if(self->updated != even) {
        for(int i = 0; i < self->speed_y; i++) {
            if(container->getParticle(x, y + 1).material != Materials::air &&
            container->getParticle(x, y + 1).material != Materials::smoke &&
            container->getParticle(x, y + 1).material != Materials::oil) {
                self->speed_y = 0;
            } else {
                swapParticles(*self, container->getParticle(x, y + 1));
                y++;
                self = &container->getParticle(x, y);
            }
        }

        if(container->getParticle(x, y + 1).material != Materials::air &&
        container->getParticle(x, y + 1).material != Materials::oil &&
        container->getParticle(x, y + 1).material != Materials::smoke) {
            if((container->getParticle(x + 1, y).material == Materials::air || container->getParticle(x + 1, y).material == Materials::oil || container->getParticle(x + 1, y).material == Materials::smoke) &&
            (container->getParticle(x - 1, y).material == Materials::air || container->getParticle(x - 1, y).material == Materials::oil || container->getParticle(x - 1, y).material == Materials::smoke))
                self->speed_x = (rand() % 2 * 2 - 1) * 5;
            else if(container->getParticle(x + 1, y).material == Materials::air || container->getParticle(x + 1, y).material == Materials::smoke || container->getParticle(x + 1, y).material == Materials::oil)
                self->speed_x = 5;
            else if(container->getParticle(x - 1, y).material == Materials::air || container->getParticle(x + 1, y).material == Materials::smoke || container->getParticle(x + 1, y).material == Materials::oil)
                self->speed_x = -1;
            else
                self->speed_x = 0;

            if(self->speed_x > 0) {
                for(int i = 0; i < self->speed_x; i++) {
                    if(container->getParticle(x + 1, y).material != Materials::air && container->getParticle(x + 1, y).material != Materials::smoke && container->getParticle(x + 1, y).material != Materials::oil)
                        break;
                    swapParticles(*self, container->getParticle(x + 1, y));
                    x++;
                    self = &container->getParticle(x, y);
                }
            } else {
                for(int i = 5; i >= self->speed_x; i--) {
                    if(container->getParticle(x - 1, y).material != Materials::air && container->getParticle(x + 1, y).material != Materials::smoke && container->getParticle(x + 1, y).material != Materials::oil)
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

void swapParticles(Particle& particle1, Particle& particle2) {
    Particle temporary_particle = particle1;
    particle1 = particle2;
    particle2 = temporary_particle;
}

bool sandSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i) {
    Particle& target_particle = container->getParticle(target_x, target_y);
    Particle& self = container->getParticle(x, y);
    if(self.updated == even && (target_particle.material == Materials::air || target_particle.material == Materials::water || target_particle.material == Materials::acid || target_particle.material == Materials::oil)) {
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
        if(particle->material == Materials::wood && rand() % 200 == 0)
            particle->material = Materials::fire;
        else if(particle->material == Materials::gunpowder && rand() % 2 == 0){
            particle->material = Materials::fire;
            particle->timer = 20 - rand() % 5;
        } else if(particle->material == Materials::oil && rand() % 8 == 0){
            particle->material = Materials::fire;
            particle->timer = 60 - rand() % 10;
        }
}


void acidCorrode(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y), &container->getParticle(x, y - 1)};
    for(Particle* particle : particles)
        if((particle->material == Materials::wood || particle->material == Materials::sand) && rand() % 20 == 0){
            container->getParticle(x, y).material = Materials::air;
            if(rand() % 10 == 0)
                particle->material = Materials::air;
            else
                particle->material = Materials::acid;
        }else if(particle->material == Materials::water && rand() % 4 == 0)
            particle->material = Materials::acid;

}

void fireWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->material == Materials::water) {
            particle->material = Materials::smoke;
            self->material = Materials::smoke;
            self->texture_color = 0;
            particle->texture_color = 0;
        }else if(particle->material == Materials::acid){
            self->material = Materials::smoke;
            self->timer = 60;
            self->texture_color = 0;
            particle->material = Materials::air;
        }
}

void lavaWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->material == Materials::water) {
            container->getParticle(x, y - 2).material = Materials::smoke;
            particle->material = Materials::stone;
            self->material = Materials::stone;
            particle->texture_color = 0;
        }else if(particle->material == Materials::acid){
            self->material = Materials::stone;
            particle->material = Materials::air;
        }
}

void lightLava(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};
    for(Particle* particle : particles)
        if(particle->material == Materials::wood && rand() % 50 == 0)
            particle->material = Materials::fire;
        else if(particle->material == Materials::gunpowder){
            particle->material = Materials::fire;
            particle->timer = 20 - rand() % 5;
        } else if(particle->material == Materials::oil && rand() % 2 == 0){
            particle->material = Materials::fire;
            particle->timer = 60 - rand() % 10;
        }
}
