#pragma once

#define ENGINE_BACKEND
#include <Engine.hxx>
#include <iostream>
#include <string.h>
using namespace engine;

#ifndef ATLAS_POS_X
	#error please define an x-axis position for your texture
	#define ATLAS_POS_X 0
#endif
#ifndef ATLAS_POS_Y
	#error please define an x-axis position for your texture
	#define ATLAS_POS_Y 0
#endif

struct __attribute((packed)) vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat texX;
	GLfloat texY;
	vertex(
		GLfloat x,
		GLfloat y,
		GLfloat z,
		GLfloat texX,
		GLfloat texY
	):
	x(x),
	y(y),
	z(z),
	texX(texX),
	texY(texY)
	{};
};

static void drawSqaure(Position2D pos,Size2D size, vertex* vertices)
{
	RenderPosition2D rp(pos);
	RenderSize2D rs(size);
	/*
	std::cout << "rp: " << rp.x << ":" << rp.y << std::endl;
	std::cout << "rs: " << rs.width << "x" << rs.height << std::endl;
	*/
	//float tfs = 0.03125;//1/32
	float tfs = 0.029411764705882;//1/34
	uint8_t xpos = ATLAS_POS_X;
	uint8_t ypos = ATLAS_POS_Y;
	float tfx = tfs*xpos;
	float tfy = tfs*ypos;

	vertices[0] = vertex(rp.x			,rp.y			,0.0,tfx	,tfy	);
	vertices[1] = vertex(rp.x+rs.width	,rp.y			,0.0,tfx+tfs,tfy	);
	vertices[2] = vertex(rp.x+rs.width	,rp.y+rs.height	,0.0,tfx+tfs,tfy+tfs);
	vertices[3] = vertex(rp.x+rs.width	,rp.y+rs.height	,0.0,tfx+tfs,tfy+tfs);
	vertices[4] = vertex(rp.x			,rp.y+rs.height	,0.0,tfx	,tfy+tfs);
	vertices[5] = vertex(rp.x			,rp.y			,0.0,tfx	,tfy	);

//	GLfloat vertices[] =
//	{
//		/* coords */rp.x			,rp.y			, 0.0f,		/* Texture mapping */tfx	, tfy		,/* lower left */
//		/* coords */rp.x+rs.width	,rp.y			, 0.0f,		/* Texture mapping */tfx+tfs, tfy		,/* lower right */
//		/* coords */rp.x+rs.width	,rp.y+rs.height	, 0.0f,		/* Texture mapping */tfx+tfs, tfy+tfs	,/* upper right */
//		/* coords */rp.x+rs.width	,rp.y+rs.height	, 0.0f,		/* Texture mapping */tfx+tfs, tfy+tfs	,/* upper right */
//		/* coords */rp.x			,rp.y+rs.height	, 0.0f,		/* Texture mapping */tfx	, tfy+tfs	,/* upper left */
//		/* coords */rp.x			,rp.y			, 0.0f,		/* Texture mapping */tfx	, tfy		,/* lower left */
//	};
}

static void RenderAll(std::vector<Object*>& Objects, uint64_t zplane)
{
	if(currentWindow == nullptr)
		throw BadCurrentWindowException(currentWindow,"["LocalClassName"] assertion failed: currentWindow != nullptr");
	//std::cout << "rendering class \"" << localClass.Name << "\" on z-plane: " << zplane << std::endl;
	GLuint AtlasUni = glGetUniformLocation(localClass.shader->ShaderProgram,"TextureAtlas");
	glUniform1i(AtlasUni,0);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	uint64_t bufferSize = Objects.size()*6*sizeof(GLfloat)*5;
	//std::cout << "buffer size: " << bufferSize << std::endl;
	GLfloat* vertices = (GLfloat*)malloc(bufferSize);
	uint64_t tvert = (uint64_t)vertices;
	for(uint64_t i = 0;i<Objects.size();i++)
	{
		Object* obj = Objects[i];
		drawSqaure(obj->Pos,obj->Size,(vertex*)tvert);
		tvert+=sizeof(vertex)*6;
	}
	//+ prep
	GLuint VertexArrayObject;//VAO
	GLuint VertexBufferObject;//VBO
	glGenVertexArrays(1,&VertexArrayObject);//? <----| keep in this order yt guy said it's important
	glGenBuffers(1,&VertexBufferObject);//?<---------|
	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,bufferSize,vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//+ draw
	glDrawArrays(GL_TRIANGLES,0,6*Objects.size());
	//+ clean up
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VertexArrayObject);
	glDeleteBuffers(1, &VertexBufferObject);
	glDisable(GL_BLEND);
	free(vertices);
}