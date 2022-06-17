#pragma once
#include "World.h"
class EditGUI
{
private:
	World& world;
	GLFWwindow& window;
	std::vector<char*> debugItems;

public:

	EditGUI(World& world, GLFWwindow& window);

	void Update();

	~EditGUI();
}; 

