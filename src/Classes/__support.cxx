#include <Engine/Shader.hxx>
#include <Engine/stb_image.h>

using namespace engine;

//,####################################################################################################################
//,####################################################################################################################
//, ███████  ██████   █████  ██    ██ ██████  ███████     ███████  ██████  ██      ██ ██████
//, ██      ██    ██ ██   ██ ██    ██ ██   ██ ██          ██      ██    ██ ██      ██ ██   ██
//, ███████ ██    ██ ███████ ██    ██ ██████  █████       ███████ ██    ██ ██      ██ ██   ██
//,      ██ ██ ▄▄ ██ ██   ██ ██    ██ ██   ██ ██               ██ ██    ██ ██      ██ ██   ██
//, ███████  ██████  ██   ██  ██████  ██   ██ ███████     ███████  ██████  ███████ ██ ██████
//,####################################################################################################################
//,####################################################################################################################
void __Shader_Generic_Object_Sqaure_Solid__onLoad(Shader* shader)
{
	//,
	//,	load texture atlas
	//,
	GLuint texture;
	{
		int imgWidth, imgHeight, colorChannelCount;
		unsigned char* bytes = stbi_load("./assets/Textures/TestAtlas.png",&imgWidth,&imgHeight,&colorChannelCount,0);
		glGenTextures(1,&texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgWidth,imgHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,bytes);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(bytes);
		GLuint AtlasUni = glGetUniformLocation(shader->ShaderProgram,"TextureAtlas");
		glUniform1i(AtlasUni,0);
	}
}

Shader __Shader_Generic_Object_Sqaure_Solid(
	"./assets/shaders/Atlas16.vert",
	"./assets/shaders/Atlas16.frag",
	"Generic Atlas 16 Sqaure Shader",
	&__Shader_Generic_Object_Sqaure_Solid__onLoad
);