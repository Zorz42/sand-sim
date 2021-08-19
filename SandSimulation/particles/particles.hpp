#pragma once

#include "particleEngine.hpp"

struct Sand : Material {
    Sand();
};

struct Water : Material {
    Water();
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Wood : Material {
    Wood();
};

struct Fire : Material {
    Fire();
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Smoke : Material {
    Smoke();
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Gunpowder : Material {
    Gunpowder();
};

struct Acid : Material {
    Acid();
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Oil : Material {
    Oil();
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Lava : Material {
    Lava();
    void update(ParticleContainer* container, int x, int y, bool even) override;
};


namespace Materials {

inline Material *sand = new Sand(), *water = new Water(), *wood = new Wood(), *fire = new Fire(), *smoke = new Smoke(), *gunpowder = new Gunpowder(), *acid = new Acid(), *oil = new Oil(), *lava = new Lava();

}
