#pragma once

#include "tigl.h"
#include <vector>
#include "Texture.h"

class VBOModel
{
public:
	std::vector<tigl::VBO*> vbos;
	std::vector<Texture*> textures;

	void Draw();
};

