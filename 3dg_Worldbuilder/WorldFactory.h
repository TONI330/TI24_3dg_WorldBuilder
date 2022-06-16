#pragma once
#include <string>
#include <vector>
#include "WorldObject.h"

class WorldFactory
{
private:
	static WorldObject* GetWorldObject(std::string line);
	static int IndexOf(std::vector<std::string> params, const char* check);

public:
	static std::vector<WorldObject*> LoadWorldObjects();
};

