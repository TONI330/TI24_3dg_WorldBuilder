#include "VBOModel.h"

void VBOModel::Draw()
{
	for (int i = 0; i < vbos.size(); i++)
	{
		if (textures[i])
			textures[i]->bind();
		//tigl::drawVertices(GL_TRIANGLES, vbos[i]);
	}
}
