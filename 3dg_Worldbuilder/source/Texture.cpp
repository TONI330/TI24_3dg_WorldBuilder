#include "Texture.h"

#include "stb_image.h"

Texture::Texture(const std::string & fileName)
{

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int width, height, bpp;
    stbi_set_flip_vertically_on_load(1);
	unsigned char* imgData = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,		//level
        GL_RGBA,		//internal format
        width,		//width
        height,		//height
        0,		//border
        GL_RGBA,		//data format
        GL_UNSIGNED_BYTE,	//data type
        imgData);		//data
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void Texture::bind()
{
    if(id) glBindTexture(GL_TEXTURE_2D, id);
}
