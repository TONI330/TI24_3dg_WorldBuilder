#pragma once
#include <glm/ext/vector_float3.hpp>

class WorldObject
{
private:
	glm::vec3 position;

public:
	virtual void Init() {};
	virtual void Draw() {};
	virtual void Update() {};

};

