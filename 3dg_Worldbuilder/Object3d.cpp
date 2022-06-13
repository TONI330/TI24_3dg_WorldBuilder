#include "Object3d.h"
#include <iostream>
#include "StaticSettings.h"

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

#if DEBUG_LEVEL == 0
	std::cout << "drawing\n";
#endif



	
}
