#include <fstream>
#include <iostream>
#include "WorldFactory.h"
#include "Object3d.h"
#include "StaticSettings.h"

int WorldFactory::IndexOf(std::vector<std::string> params, const char* check)
{
    for (int i = 0; i < params.size(); i++)
    {
        if (params[i] == check)
            
            return i;
    }
    return -1;
}

WorldObject* WorldFactory::GetWorldObject(std::string line)
{
    std::vector<std::string> params = ObjModel::split(line, " ");
    if (IndexOf(params, "3d") != -1)
    {
        if(IndexOf(params, "m=") != -1)
            return new Object3d(params[IndexOf(params, "m=") + 1]);
#if DEBUG_LEVEL <= 3
        std::cout << "3d object in save file has no mesh";
#endif
    }
}

std::vector<WorldObject*> WorldFactory::LoadWorldObjects()
{
    auto objects = std::vector<WorldObject*>();
    std::ifstream pFile(SAVE_FILE);
    if (!pFile.is_open())
    {
#if DEBUG_LEVEL <= 2
        std::cout << "Save file not found, creating empty world";
#endif
        return objects;
    }

    while (!pFile.eof())
    {
        std::string line;
        std::getline(pFile, line);
        std::vector<std::string> params = ObjModel::split(line, " ");
        if (params[0] != "wo") //load only worldobjects
            continue;
        auto object = GetWorldObject(line);
        int posIndex = IndexOf(params, "p=");
        if (posIndex != -1)
        {
            object->GetTransform()->position.x = atoi(params[posIndex + 1].c_str());
            object->GetTransform()->position.y = atoi(params[posIndex + 2].c_str());
            object->GetTransform()->position.z = atoi(params[posIndex + 3].c_str());
        }
        objects.push_back(object);
    }

    return objects;
}


