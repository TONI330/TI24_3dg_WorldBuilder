#pragma once
#include "WorldObject.h"
class ObjectLight :
    public WorldObject
{



public:
    int lightId;

    ObjectLight(int& lightsAmount, const int lightId);
    ObjectLight(const int lightId);
    void Update() override;
    void Draw() override;

};

