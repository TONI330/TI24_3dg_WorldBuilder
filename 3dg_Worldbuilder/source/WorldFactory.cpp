#include <fstream>
#include <iostream>
#include "WorldFactory.h"
#include "Object3d.h"
#include "StaticSettings.h"
#include "StringUtil.h"

WorldObject* WorldFactory::GetWorldObject(std::string line)
{
    std::vector<std::string> params = StringUtil::split(line, " ");
    if (StringUtil::IndexOf(params, "3d") != -1)
    {
        if(StringUtil::IndexOf(params, "m=") != -1)
            return new Object3d(params[StringUtil::IndexOf(params, "m=") + 1]);
#if DEBUG_LEVEL <= DEBUG_LEVEL_ERROR
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
#if DEBUG_LEVEL <= DEBUG_LEVEL_WARNING
        std::cout << "Save file not found, creating empty world";
#endif
        return objects;
    }

    while (!pFile.eof())
    {
        std::string line;
        std::getline(pFile, line);
        std::vector<std::string> params = StringUtil::split(line, " ");
        if (params[0] != "wo") //load only worldobjects
            continue;
        auto object = GetWorldObject(line);
        int posIndex = StringUtil::IndexOf(params, "p=");
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


