#pragma once
#include <vector>
#include "WorldObject.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "FpCam.h"
#include <iostream>


class World
{
private:
	float time;
	FpCam& camera;
	GLFWwindow& window;
	
	void RotateAround(glm::vec3 center, glm::vec3 target, float angle_radians);

public:
	std::vector<WorldObject *> objects;

	World(GLFWwindow& window, FpCam& camera);
	void AddWorldObject(WorldObject* object);
	void UpdateWorld();
	void DrawWorld();

	template<class T>
	std::vector<T*> GetWorldObjects()
	{
		std::vector<T*> found;
		for (auto object : objects)
		{
			auto derived = dynamic_cast<T*>(object);
			if (derived)
				found.push_back(derived);
		}
		return found;
	}

	~World();

};
