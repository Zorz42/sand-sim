#include "particles.hpp"

Sand::Sand() {
    color = {{245, 210, 92}, {237, 205, 88}, {239, 207, 90}, {235, 203, 86}, {231, 200, 84}};
    random_spawn = 20;
    type = MaterialType::POWDER;
}

Wood::Wood() {
    color = {{155, 116, 54}, {153, 115, 53}, {157, 117, 55}, {151, 111, 51}};
    type = MaterialType::SOLID;
    flamability = 0.005;
    burn_time = 500;
}

Gunpowder::Gunpowder() {
    color = {{37, 37, 37}};
    random_spawn = 20;
    type = MaterialType::POWDER;
    flamability = 0.5;
    burn_time = 20;
}


Water::Water() {
    color = {{53, 145, 173}, {53, 147, 176}, {47, 140, 169}};
    random_spawn = 20;
    type = MaterialType::LIQUID;
}

void Water::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);

    if(self->updated != even) {
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
    random_spawn = 20;
    type = MaterialType::LIQUID;
    flamability = 0.125;
    burn_time = 120;
}

void Oil::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);

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
    random_spawn = 20;
    type = MaterialType::LIQUID;
    burn_factor = 2;
}

void Lava::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);

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
    
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->material == Materials::water) {
            container->getParticle(x, y - 2).material = Materials::smoke;
            particle->material = Materials::stone;
            self->material = Materials::stone;
            particle->texture_color = 0;
        } else if(particle->material == Materials::acid) {
            self->material = Materials::stone;
            particle->material = Materials::air;
        }
    
    for(Particle* particle : particles)
        if(particle->material->flamability != 0) {
            if((double)rand() / RAND_MAX < particle->material->flamability * self->material->burn_factor) {
                particle->timer = particle->material->burn_time - rand() % (particle->material->burn_time / 10);
                particle->material = Materials::fire;
            }
        }

}

Fire::Fire() {
    color = {{222, 70, 12}, {222, 120, 16}, {222, 125, 16}, {222, 127, 16}, {222, 130, 16}, {222, 182, 16}};
    type = MaterialType::SOLID;
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

    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};
    for(Particle* particle : particles)
        if(particle->material->flamability != 0) {
            if((double)rand() / RAND_MAX < particle->material->flamability * self->material->burn_factor) {
                particle->timer = particle->material->burn_time - rand() % (particle->material->burn_time / 10);
                particle->material = Materials::fire;
            }
        }

    for(Particle* particle : particles)
        if(particle->material == Materials::water) {
            particle->material = Materials::smoke;
            self->material = Materials::smoke;
            self->texture_color = 0;
            particle->texture_color = 0;
        } else if(particle->material == Materials::acid) {
            self->material = Materials::smoke;
            self->timer = 60;
            self->texture_color = 0;
            particle->material = Materials::air;
        }

    if(container->getParticle(x, y - 1).material == Materials::air && rand() % 10 == 0)
        container->getParticle(x, y - 1).material = Materials::smoke;
}

Smoke::Smoke() {
    color = {{65, 65, 59}, {61, 60, 54}, {69, 70, 64}};
    random_spawn = 20;
    type = MaterialType::GAS;
}

void Smoke::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);

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
    random_spawn = 20;
    type = MaterialType::LIQUID;
}

void Acid::update(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);

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
