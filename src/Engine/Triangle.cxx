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

namespace engine
{
	void drawTriangle(Position2D corners[3],rgba color,bool defaultShader)
	{
		if(currentWindow == nullptr)
			throw BadCurrentWindowException(currentWindow,"[ENGINE] assertion failed: currentWindow != nullptr");
		if(defaultShader)
			shaders::BasicRGBShader2D->activate();
		RenderPosition2D rp0(corners[0]);
		RenderPosition2D rp1(corners[1]);
		RenderPosition2D rp2(corners[2]);
		GLfloat vertices[] =
		{
			rp0.x,rp0.y, 0.0f,	color.Red,color.Green,color.Blue,color.Alpha,
			rp1.x,rp1.y, 0.0f,	color.Red,color.Green,color.Blue,color.Alpha,
			rp2.x,rp2.y, 0.0f,	color.Red,color.Green,color.Blue,color.Alpha,
		};

		//+ prep
		GLuint VertexArrayObject;//VAO
		GLuint VertexBufferObject;//VBO
		glGenVertexArrays(1,&VertexArrayObject);//? <----| keep in this order yt guy said it's important
		glGenBuffers(1,&VertexBufferObject);//?<---------|

		glBindVertexArray(VertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)12);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//+ draw
		glDrawArrays(GL_TRIANGLES,0,3);

		//+ clean up
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);

		glDeleteVertexArrays(1, &VertexArrayObject);
		glDeleteBuffers(1, &VertexBufferObject);
	}
}