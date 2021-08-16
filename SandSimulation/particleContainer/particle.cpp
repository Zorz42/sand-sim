#include "particleContainer.hpp"

MaterialType Particle::getType() {
    return type;
}

void Particle::setType(MaterialType material_type) {
    if(type != material_type) {
        changed_type = true;
        type = material_type;
    }
}

void Particle::operator=(Particle& x) {
    setType(x.getType());
    speed_x = x.speed_x;
    speed_y = x.speed_y;
    timer = x.timer;
    updated = x.updated;
}

void Particle::markUpdated() {
    changed_type = false;
}

bool Particle::hasChangedType() {
    return changed_type;
}

const Material& Particle::getUniqueMaterial() {
    return getMaterialByType(type);
}
