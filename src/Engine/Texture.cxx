#include <Engine/platform.h>

//, ███████ ███    ██  ██████  ██ ███    ██ ███████
//, ██      ████   ██ ██       ██ ████   ██ ██
//, █████   ██ ██  ██ ██   ███ ██ ██ ██  ██ █████
//, ██      ██  ██ ██ ██    ██ ██ ██  ██ ██ ██
//, ███████ ██   ████  ██████  ██ ██   ████ ███████
#include <Engine/Window.hxx>
#include <Engine/Except.hxx>
#include <Engine/Position.hxx>
#include <Engine/RGBA.hxx>

//, ███████ ██    ██ ███████ ████████ ███████ ███    ███
//, ██       ██  ██  ██         ██    ██      ████  ████
//, ███████   ████   ███████    ██    █████   ██ ████ ██
//,      ██    ██         ██    ██    ██      ██  ██  ██
//, ███████    ██    ███████    ██    ███████ ██      ██
#include <iostream>

#include <Engine/stb_image.h>

namespace engine
{
	void drawSqaureTexture(Position2D pos,Size2D size, char* file,bool defaultShader = false)
	{
		if(currentWindow == nullptr)
			throw BadCurrentWindowException(currentWindow,"[ENGINE] assertion failed: currentWindow != nullptr");
		if(defaultShader)
			shaders::BasicTextureShader2D->activate();
		RenderPosition2D rp(pos);
		RenderSize2D rs(size);
		/*
		std::cout << "rp: " << rp.x << ":" << rp.y << std::endl;
		std::cout << "rs: " << rs.width << "x" << rs.height << std::endl;
		*/
		GLfloat vertices[] =
		{
			/* coords */rp.x			,rp.y			, 0.0f,		/* Texture mapping */0.0f, 0.0f,/* lower left */
			/* coords */rp.x+rs.width	,rp.y			, 0.0f,		/* Texture mapping */1.0f, 0.0f,/* lower right */
			/* coords */rp.x+rs.width	,rp.y+rs.height	, 0.0f,		/* Texture mapping */1.0f, 1.0f,/* upper right */
			/* coords */rp.x+rs.width	,rp.y+rs.height	, 0.0f,		/* Texture mapping */1.0f, 1.0f,/* upper right */
			/* coords */rp.x			,rp.y+rs.height	, 0.0f,		/* Texture mapping */0.0f, 1.0f,/* upper left */
			/* coords */rp.x			,rp.y			, 0.0f,		/* Texture mapping */0.0f, 0.0f,/* lower left */
		};

		//,
		//,	load texture
		//,
		GLuint texture;
		{
			int imgWidth, imgHeight, colorChannelCount;
			unsigned char* bytes = stbi_load(file,&imgWidth,&imgHeight,&colorChannelCount,0);
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

			GLuint tex0Uni = glGetUniformLocation(shaders::BasicTextureShader2D->ShaderProgram,"tex0");
			glUniform1i(tex0Uni,0);
		}

		//+ prep
		GLuint VertexArrayObject;//VAO
		GLuint VertexBufferObject;//VBO
		glGenVertexArrays(1,&VertexArrayObject);//? <----| keep in this order yt guy said it's important
		glGenBuffers(1,&VertexBufferObject);//?<---------|

		glBindVertexArray(VertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//+ draw
		glDrawArrays(GL_TRIANGLES,0,6);

		//+ clean up
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);

		glDeleteVertexArrays(1, &VertexArrayObject);
		glDeleteBuffers(1, &VertexBufferObject);
		glDeleteTextures(1,&texture);
	}
}