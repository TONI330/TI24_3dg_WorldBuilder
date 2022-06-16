#pragma once
#include "WorldObject.h"
class ObjectLight :
    public WorldObject
{



public:
    int lightId;

    ObjectLight(int& lightsAmount);
    void Update() override;
    void Draw() override;

};

