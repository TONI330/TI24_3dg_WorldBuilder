#include "Object3d.h"
#include <iostream>
#include "StaticSettings.h"
#include <glm/ext/matrix_transform.hpp>
#include "tigl.h"
#include "Transform.h"


glm::mat4 Object3d::TransToModel(const Transform& transform)
{
	auto modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, transform.position);
	modelMatrix = glm::scale(modelMatrix, transform.scale);


	auto rotation = transform.rotation;
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	
	return modelMatrix;
}

void Object3d::Scale(const float scale)
{
	this->GetTransform()->scale.x *= scale;
	this->GetTransform()->scale.y *= scale;
	this->GetTransform()->scale.z *= scale;
}

void Object3d::Scale(const glm::vec3 scale)
{
	this->GetTransform()->scale.x *= scale.x;
	this->GetTransform()->scale.x *= scale.y;
	this->GetTransform()->scale.x *= scale.z;
}

Object3d::Object3d(std::string meshPath)
{
	mesh = new ObjModel(meshPath);
	this->name = "nameless";
}

Object3d::Object3d(std::string meshPath, std::string constrName)
{
	this->name = constrName;
	mesh = new ObjModel(meshPath);
}

void Object3d::Update()
{
	WorldObject::Update();
}

void Object3d::Draw()
{
	WorldObject::Draw();
	
	tigl::shader->setModelMatrix(this->TransToModel(*this->GetTransform()));
	
	mesh->draw();

#if DEBUG_LEVEL == 0
	//std::cout << "position x: " << this->GetTransform().position;
#endif



	
}