#pragma once
#include "WorldObject.h"
#include <string>
#include "ObjModel.h"
#include "VBOModel.h"
class Object3d :
    public WorldObject
{
private:
    ObjModel* mesh;

public:
    Object3d(std::string meshPath);
    Object3d(std::string meshPath, std::string name);

    glm::mat4 TransToModel(const Transform& transform);
    void Scale(const float scale);
    void Scale(const glm::vec3 scale);

    void Update() override;
    void Draw() override;

};

