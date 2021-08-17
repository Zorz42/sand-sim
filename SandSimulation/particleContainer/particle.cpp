#include "particleContainer.hpp"

Material* Particle::getMaterial() {
    return material;
}

void Particle::setMaterial(Material* material_to_be) {
    if(material != material_to_be) {
        changed_type = true;
        material = material_to_be;
    }
}

void Particle::operator=(Particle& x) {
    setMaterial(x.getMaterial());
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
