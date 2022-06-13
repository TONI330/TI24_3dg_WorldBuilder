#pragma once
#include <glm/ext/vector_float3.hpp>
#include <string>
#include "Transform.h"

class WorldObject
{
private:
	Transform* transform = new Transform;

public:
	WorldObject();

	std::string name;

	Transform* GetTransform();
	virtual void Init() {};
	virtual void Draw() {};
	virtual void Update() {};

	~WorldObject();
};

