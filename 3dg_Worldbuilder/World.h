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

public:
	std::vector<WorldObject *> objects;

	World(GLFWwindow& window, FpCam& camera);
	void AddWorldObject(WorldObject* object);
	void UpdateWorld();
	void DrawWorld();

};
