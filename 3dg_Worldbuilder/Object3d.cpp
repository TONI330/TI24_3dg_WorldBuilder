#include "Object3d.h"
#include <iostream>
#include "StaticSettings.h"
#include <glm/ext/matrix_transform.hpp>
#include "tigl.h"
#include "Transform.h"

Object3d::Object3d(std::string meshPath)
{
	mesh = new ObjModel(meshPath);
	name = "nameless";
}

Object3d::Object3d(std::string meshPath, std::string name) 
{
	mesh = new ObjModel(meshPath);
}

void Object3d::Update()
{
	WorldObject::Update();
}

void Object3d::Draw()
{
	WorldObject::Draw();
	auto modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, this->GetTransform()->scale);
	modelMatrix = glm::translate(modelMatrix, this->GetTransform()->position);

	auto rotation = this->GetTransform()->rotation;
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));

	tigl::shader->setModelMatrix(modelMatrix);

	mesh->draw();

#if DEBUG_LEVEL == 0
	//std::cout << "position x: " << this->GetTransform().position;
#endif



	
}
