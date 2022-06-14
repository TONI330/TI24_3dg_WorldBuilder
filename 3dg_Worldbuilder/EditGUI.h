#pragma once
#include "World.h"
class EditGUI
{
private:
	World& world;
	GLFWwindow& window;

public:
	EditGUI(World& world, GLFWwindow& window);
	void Update();

	~EditGUI();
}; 

