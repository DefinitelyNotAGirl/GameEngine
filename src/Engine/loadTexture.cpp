#include <Engine.hxx>
#include <Engine/stb_image.h>

namespace engine
{

	void simpleLoadTexture(char* file, GLenum slot)
	{
		GLuint texture;
		int imgWidth, imgHeight, colorChannelCount;
		unsigned char* bytes = stbi_load(file,&imgWidth,&imgHeight,&colorChannelCount,0);
		glGenTextures(1,&texture);
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D,texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgWidth,imgHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,bytes);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(bytes);
	}
}