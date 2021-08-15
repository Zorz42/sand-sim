#pragma once

#include "materialClass.hpp"

class container{
    particle particleMap[10][10];
    void initMap();
    void updateAll();
};