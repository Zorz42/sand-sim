#pragma once

#include "materialClass.hpp"

class particle_container{
    Material** particleMap = nullptr;
    int arraySizeX, arraySizeY;
public:
    void initMap();
    void updateAll();
    particle_container(int x_size, int y_size);
    Material* getParticle(unsigned short x, unsigned short y);
};