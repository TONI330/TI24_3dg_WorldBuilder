#pragma once
#include "WorldObject.h"
#include <string>
#include "ObjModel.h"
class Object3d :
    public WorldObject
{
private:
    ObjModel* mesh;

public:
    Object3d(std::string mesh);

    void Update() override;
    void Draw() override;

};

