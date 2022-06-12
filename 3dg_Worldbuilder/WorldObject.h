#pragma once
#include <glm/ext/vector_float3.hpp>
#include <string>

class WorldObject
{
private:
	glm::vec3 position;

public:
	WorldObject();

	std::string name;

	virtual void Init() {};
	virtual void Draw() {};
	virtual void Update() {};

};

