#include "WorldObject.h"

WorldObject::WorldObject()
{
	transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
	transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	transform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transform* WorldObject::GetTransform()
{
	return transform;
}

WorldObject::~WorldObject()
{
	delete(this->transform);
}
