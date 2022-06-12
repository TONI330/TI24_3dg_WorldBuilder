#include "Object3d.h"
#include <iostream>

Object3d::Object3d(std::string meshPath)
{
	mesh = new ObjModel(meshPath);
	name = "object";
}

void Object3d::Update()
{
	WorldObject::Update();
}

void Object3d::Draw()
{
	WorldObject::Draw();

	

	mesh->draw();
	std::cout << "drawing\n";
}
