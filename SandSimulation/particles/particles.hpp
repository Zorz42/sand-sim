#pragma once

#include "particleEngine.hpp"

struct Sand : Material {
    Sand() : Material({{245, 210, 92}, {237, 205, 88}, {239, 207, 90}, {235, 203, 86}, {230, 200, 84}}, 0.08, 80) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Water : Material {
    Water() : Material({{52, 145, 173}, {53, 147, 176}, {47, 140, 169}}, 0.08, 80) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Wood : Material {
    Wood() : Material({{155, 116, 54}, {154, 115, 53}, {156, 117, 55}, {150, 111, 51}}, 0, 0) {}
};

struct Fire : Material {
    Fire() : Material({{222, 70, 12}, {222, 120, 16}, {222, 125, 16}, {222, 127, 16}, {222, 130, 16}, {222, 182, 16}}, 0, 20) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Smoke : Material {
    Smoke() : Material({{65, 65, 59}, {60, 60, 54}, {70, 70, 64}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Gunpowder : Material {
    Gunpowder() : Material({{36, 36, 36}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Acid : Material {
    Acid() : Material({{31, 255, 61}, {32, 255, 62}, {31, 255, 61}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Oil : Material {
    Oil() : Material({{10, 10, 10}, {12, 12, 12}, {9, 9, 9}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};

struct Lava : Material {
    Lava() : Material({{252, 91, 16}, {252, 95, 16}, {252, 97, 16}, {252, 102, 16}, {252, 142, 16}}, 0.08, 40) {}
    void update(ParticleContainer* container, int x, int y, bool even) override;
};


namespace Materials {

inline Material *sand = new Sand(), *water = new Water(), *wood = new Wood(), *fire = new Fire(), *smoke = new Smoke(), *gunpowder = new Gunpowder(), *acid = new Acid(), *oil = new Oil(), *lava = new Lava();

}
