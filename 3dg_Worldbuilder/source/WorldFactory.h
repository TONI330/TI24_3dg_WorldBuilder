#pragma once
#include <string>
#include <vector>
#include "WorldObject.h"

class WorldFactory
{
private:
	static WorldObject* GetWorldObject(std::string line);

public:
	static std::vector<WorldObject*> LoadWorldObjects();
};

