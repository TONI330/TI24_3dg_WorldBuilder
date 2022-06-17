#include "ObjModel.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "tigl.h"
#include "Texture.h"
#include "StaticSettings.h"
#include "StringUtil.h"

using tigl::Vertex;

/**
* Loads an object model
*/
ObjModel::ObjModel(const std::string &fileName)
{
#if DEBUG_LEVEL <= DEBUG_LEVEL_INFO
	std::cout << "Loading " << fileName << std::endl;
#endif
	std::string dirName = fileName;
	if(dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if(dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if(fileName == dirName)
		dirName = "";


	std::ifstream pFile(fileName.c_str());

	if (!pFile.is_open())
	{
#if DEBUG_LEVEL <= DEBUG_LEVEL_ERROR
		std::cout << "Could not open file " << fileName << std::endl;
#endif
		return;
	}

	ObjGroup* currentGroup = new ObjGroup();
	currentGroup->materialIndex = -1;


	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = StringUtil::cleanLine(line);
		if(line == "" || line[0] == '#') //skip empty or commented line
			continue;

		std::vector<std::string> params = StringUtil::split(line, " ");
		params[0] = StringUtil::toLower(params[0]);

		if(params[0] == "v")
			vertices.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if(params[0] == "vn")
			normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if(params[0] == "vt")
			texcoords.push_back(glm::vec2((float)atof(params[1].c_str()), (float)atof(params[2].c_str())));
		else if(params[0] == "f")
		{
			for(size_t ii = 4; ii <= params.size(); ii++)
			{
				Face face;

				for(size_t i = ii-3; i < ii; i++)	//magische forlus om van quads triangles te maken ;)
				{
					Vertex vertex;
					std::vector<std::string> indices = StringUtil::split(params[i == (ii-3) ? 1 : i], "/");
					if (indices.size() >= 1)	//er is een positie
						vertex.position = atoi(indices[0].c_str()) - 1;
					if(indices.size() == 2)		//alleen texture
						vertex.texcoord = atoi(indices[1].c_str())-1;
					if(indices.size() == 3)		//v/t/n of v//n
					{
						if( indices[1] != "")
							vertex.texcoord = atoi(indices[1].c_str())-1;
						vertex.normal = atoi(indices[2].c_str())-1;
					}
					face.vertices.push_back(vertex);
				}
				currentGroup->faces.push_back(face);
			}
		}
		else if(params[0] == "s")
		{//smoothing groups
		}
        else if(params[0] == "mtllib")
        {
            loadMaterialFile(dirName + "/" + params[1], dirName);
        }
		else if(params[0] == "usemtl")
		{
			if(currentGroup->faces.size() != 0)
				groups.push_back(currentGroup);
			currentGroup = new ObjGroup();
			currentGroup->materialIndex = -1;

			for(size_t i = 0; i < materials.size(); i++)
			{
				MaterialInfo* info = materials[i];
				if(info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if (currentGroup->materialIndex == -1) {
#if DEBUG_LEVEL <= DEBUG_LEVEL_ERROR
				std::cout << "Could not find material name " << params[1] << std::endl;
#endif
			}
		}
	}
	groups.push_back(currentGroup);
}

ObjModel::~ObjModel(void)
{
}

VBOModel* ObjModel::ToVBOModel()
{
	VBOModel* model = new VBOModel();
	for (const auto& group : groups)
	{
		model->textures.push_back(
			materials[group->materialIndex]->texture);
		std::vector<tigl::Vertex> VBOvertices;

		for (const auto& face : group->faces)
		{
			for (auto vertex : face.vertices)
			{
				VBOvertices.push_back(tigl::Vertex::PTN(vertices[vertex.position], texcoords[vertex.texcoord], normals[vertex.normal]));
			}
		}
		//model->vbos.push_back(tigl::createVbo(VBOvertices));
	}
	return model;
}

void ObjModel::draw()
{
	std::vector<tigl::Vertex> vertexList;
	for (const auto group : groups) 
	{
			materials[group->materialIndex]->texture->bind();
		
		for (const auto& face : group->faces)
		{
			for (auto vertex : face.vertices)
			{
				vertexList.push_back(tigl::Vertex::PTN(vertices[vertex.position], texcoords[vertex.texcoord], normals[vertex.normal]));
			}
		}
		tigl::drawVertices(GL_TRIANGLES, vertexList);
	}
}

void ObjModel::loadMaterialFile( const std::string &fileName, const std::string &dirName )
{
#if DEBUG_LEVEL <= DEBUG_LEVEL_INFO
	std::cout << "Loading " << fileName << std::endl;
#endif
	std::ifstream pFile(fileName.c_str());
	if (!pFile.is_open())
	{
#if DEBUG <= 2
		std::cout << "Could not open file " << fileName << std::endl;
#endif
		return;
	}
	MaterialInfo* currentMaterial = NULL;

	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = StringUtil::cleanLine(line);
		if(line == "" || line[0] == '#')
			continue;

		std::vector<std::string> params = StringUtil::split(line, " ");
		params[0] = StringUtil::toLower(params[0]);

		if(params[0] == "newmtl")
		{
			if(currentMaterial != NULL)
			{
				materials.push_back(currentMaterial);
			}
			currentMaterial = new MaterialInfo();
			currentMaterial->name = params[1];
		}
		else if(params[0] == "map_kd")
		{
			std::string tex = params[1];
			if (tex.find("/"))
				tex = tex.substr(tex.rfind("/") + 1);
			if (tex.find("\\"))
				tex = tex.substr(tex.rfind("\\") + 1);
			//TODO
			currentMaterial->texture = new Texture(dirName + "/" + tex);
			currentMaterial->texture->bind();
		} 
		else if (params[0] == "kd")
		{//TODO, diffuse color
		}
		else if (params[0] == "ka")
		{//TODO, ambient color
		}
		else if (params[0] == "ks")
		{//TODO, specular color
		}
		else if (
			params[0] == "illum" || 
			params[0] == "map_bump" || 
			params[0] == "map_ke" || 
			params[0] == "map_ka" ||
			params[0] == "map_d" ||
			params[0] == "d" ||
			params[0] == "ke" ||
			params[0] == "ns" ||
			params[0] == "ni" ||
			params[0] == "td" ||
			params[0] == "tf" ||
			params[0] == "tr" ||
			false)
		{
			//these values are usually not used for rendering at this time, so ignore them
		}
		else
		{
#if DEBUG <= 2
			std::cout << "Didn't parse " << params[0] << " in material file" << std::endl;
#endif
		}			
	}
	if(currentMaterial != NULL)
		materials.push_back(currentMaterial);
}

ObjModel::MaterialInfo::MaterialInfo()
{
	texture = NULL;
}


