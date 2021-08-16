#include "particleContainer.hpp"

void swapParticles(Particle& particle1, Particle& particle2);
bool sandSwapLeftDown(int& x, int& y, ParticleContainer* container, bool even, int i);
bool sandSwapRightDown(int& x, int& y, ParticleContainer* container, bool even, int i);
bool waterSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i);
void lightFire(int x, int y, ParticleContainer* container);
void fireWaterContact(int x, int y, ParticleContainer* container);
bool smokeSwapLeftUp(int& x, int& y, ParticleContainer* container, bool even, int i);
bool smokeSwapRightUp(int& x, int& y, ParticleContainer* container, bool even, int i);
bool smokeSwapLeft(int& x, int& y, ParticleContainer* container, bool even, int i);
bool smokeSwapRight(int& x, int& y, ParticleContainer* container, bool even, int i);

Material materials[(int)MaterialType::NUM_MATERIALS];

const Material& getMaterialByType(MaterialType type) {
    return materials[(int)type];
}

Material::Material(sf::Color color, float constant_force, int randomSpawn, void (*update)(ParticleContainer* container, int x, int y, bool even)) : color(color), constant_force(constant_force), randomSpawn(randomSpawn), update(update) {}


void sandUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getUniqueMaterial().constant_force;

    int i = 0;
    while(i < self->speed_y) {
        Particle& particle_below = container->getParticle(x, y + 1);
        if(self->updated == even) {
            if(particle_below.getType() == MaterialType::AIR || particle_below.getType() == MaterialType::WATER) {
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
}

void waterUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getUniqueMaterial().constant_force;
    if(self->timer == 0)
        self->timer = 3600;

    if(container->getParticle(x, y - 1).getType() == MaterialType::AIR)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1){
        self->speed_y = 0;
        self->speed_x = 1;
        self->setType(MaterialType::AIR);
        self->timer = 0;
    }

    bool moved = false;
    int i = 0;
    if(self->updated == even){
        while(i < self->speed_y && self->updated == even) {
            Particle& particle_below = container->getParticle(x, y + 1);
            if(particle_below.getType() == MaterialType::AIR || particle_below.getType() == MaterialType::SMOKE) {
                swapParticles(*self, particle_below);
                moved = true;
                if(i + 1 > particle_below.speed_y)
                    particle_below.updated = !even;
                y++;
                self = &container->getParticle(x, y);

            }else if(!waterSwap(container, x, y, x - 1, y + 1, even, i)){
                if(!waterSwap(container, x, y, x + 1, y + 1, even, i)) {
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
                if(!waterSwap(container, x, y, x - 1, y, even, i))
                    self->speed_x = -1;
                i++;
            }
            i = 0;
            while(i < -1 * self->speed_x) {
                if(!waterSwap(container, x, y, x + 1, y, even, i))
                    self->speed_x = 1;
                i++;
            }
        }
    }
}

void fireUpdate(ParticleContainer* container, int x, int y, bool even) {
    Particle* self = &container->getParticle(x, y);
    if(self->timer == 0)
        self->timer = 300 + rand() % 150;
    if(self->timer == 1)
        self->setType(MaterialType::AIR);
    self->timer--;

    lightFire(x, y, container);
    fireWaterContact(x, y, container);
}

void smokeUpdate(ParticleContainer* container, int x, int y, bool even){
    Particle* self = &container->getParticle(x, y);
    self->speed_y *= 0.995;
    self->speed_x *= 0.995;
    self->speed_y += self->getUniqueMaterial().constant_force;
    if(self->timer == 0)
        self->timer = 3600;

    if(container->getParticle(x, y + 1).getType() == MaterialType::AIR)
        self->timer--;
    else
        self->timer++;

    if(self->timer == 1){
        self->speed_y = 0;
        self->speed_x = 1;
        self->setType(MaterialType::AIR);
        self->timer = 0;
    }

    bool moved = false;
    int i = 0;
    if(self->updated == even){
        while(i < self->speed_y && self->updated == even) {
            Particle& particle_above = container->getParticle(x, y - 1);
            if(particle_above.getType() == MaterialType::AIR) {
                swapParticles(*self, particle_above);
                moved = true;
                if(i + 1 > particle_above.speed_y)
                    particle_above.updated = !even;
                y--;
                self = &container->getParticle(x, y);

            }else if(!smokeSwapLeftUp(x, y, container, even, i)){
                if(!smokeSwapRightUp(x, y, container, even, i)) {
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
                if(!smokeSwapLeft(x, y, container, even, i))
                    self->speed_x = -1;
                i++;
            }
            i = 0;
            while(i < -1 * self->speed_x) {
                if(!smokeSwapRight(x, y, container, even, i))
                    self->speed_x = 1;
                i++;
            }
        }
    }
}


void initMaterials() {
    materials[(int)MaterialType::AIR] = Material({90, 90, 90}, 0, 1);
    materials[(int)MaterialType::SAND] = Material({237, 205, 88}, 0.08, 80, &sandUpdate);
    materials[(int)MaterialType::WATER] = Material({52, 145, 173}, 0.05, 80, &waterUpdate);
    materials[(int)MaterialType::WOOD] = Material({150, 111, 51}, 0, 1);
    materials[(int)MaterialType::FIRE] = Material({222, 91, 16}, 0, 20, &fireUpdate);
    materials[(int)MaterialType::STONE] = Material({133, 133, 133}, 0, 1);
}

void swapParticles(Particle& particle1, Particle& particle2) {
    Particle temporary_particle = particle1;
    particle1 = particle2;
    particle2 = temporary_particle;
}

bool sandSwapLeftDown(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x, y).updated == even && (container->getParticle(x - 1, y + 1).getType() == MaterialType::AIR || container->getParticle(x - 1, y + 1).getType() == MaterialType::WATER)) {
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
    if(container->getParticle(x, y).updated == even && (container->getParticle(x + 1, y + 1).getType() == MaterialType::AIR || container->getParticle(x + 1, y + 1).getType() == MaterialType::WATER)) {
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

bool waterSwap(ParticleContainer* container, int& x, int& y, int target_x, int target_y, bool even, int i) {
    Particle& target_particle = container->getParticle(target_x, target_y);
    Particle& self = container->getParticle(x, y);
    if(target_particle.getType() == MaterialType::AIR) {
        swapParticles(self, target_particle);
        if(i + 1 > self.speed_x)
            target_particle.updated = !even;
        x = target_x;
        y = target_y;
        return true;
    }
    else
        return false;
}

void lightFire(int x, int y, ParticleContainer* container) {
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};
    for(Particle* particle : particles)
        if(particle->getType() == MaterialType::WOOD && rand() % 200 == 0)
            particle->setType(MaterialType::FIRE);
}

void fireWaterContact(int x, int y, ParticleContainer* container) {
    Particle* self = &container->getParticle(x, y);
    Particle* particles[] = {&container->getParticle(x, y + 1), &container->getParticle(x, y - 1), &container->getParticle(x + 1, y), &container->getParticle(x - 1, y)};

    for(Particle* particle : particles)
        if(particle->getType() == MaterialType::WATER){
            particle->setType(MaterialType::SMOKE);
            self->setType(MaterialType::SMOKE);
        }
}




bool smokeSwapLeftUp(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x - 1, y - 1).getType() == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x - 1, y - 1));
        if(i + 1 > container->getParticle(x, y).speed_y)
            container->getParticle(x - 1, y - 1).updated = !even;
        y--;
        x--;
        return true;
    }
    else
        return false;
}

bool smokeSwapRightUp(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x + 1, y - 1).getType() == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x + 1, y - 1));
        if(i + 1 > container->getParticle(x, y).speed_y)
            container->getParticle(x + 1, y - 1).updated = !even;
        y--;
        x++;
        return true;
    }
    else
        return false;
}

bool smokeSwapLeft(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x - 1, y).getType() == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x - 1, y));
        if(i + 1 > container->getParticle(x, y).speed_x)
            container->getParticle(x - 1, y).updated = !even;
        x--;
        return true;
    }
    else
        return false;
}

bool smokeSwapRight(int& x, int& y, ParticleContainer* container, bool even, int i){
    if(container->getParticle(x + 1, y).getType() == MaterialType::AIR) {
        swapParticles(container->getParticle(x, y), container->getParticle(x + 1, y));
        if(i + 1 > container->getParticle(x, y).speed_x)
            container->getParticle(x + 1, y).updated = !even;
        x++;
        return true;
    }
    else
        return false;
}

