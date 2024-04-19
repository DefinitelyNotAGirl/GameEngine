#define ENGINE_BACKEND
#include <Engine.hxx>
#include <iostream>
#include <Engine/stb_image.h>
using namespace engine;

static void PushObject(Object* obj);
static void LoopAll(std::vector<Object*>& Objects);
static void RenderAll(std::vector<Object*>& Objects);

#define LocalClassName "Class.Map.Background"
static Shader LocalShader(
	"./assets/shaders/Background.vert",
	"./assets/shaders/sunset.frag",
	LocalClassName".Shader",
	nullptr
);
static LightingProperties_T LightingProperties(
	rgba(0,0,0,0),
	0.0,
	0.2,
	rgba(0xFF,0xFF,0xFF,0xFF),
	0.0
);
static ObjectClass localClass(
	LocalClassName,
	nullptr,
	&LocalShader,
	&LightingProperties,
	&RenderAll,
	&LoopAll,
	&PushObject
);

extern rgba lightColor;

static void drawSqaure(Position2D pos,Size2D size)
{
	if(currentWindow == nullptr)
		throw BadCurrentWindowException(currentWindow,"["LocalClassName"] assertion failed: currentWindow != nullptr");
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
	//+ prep
	GLuint VertexArrayObject;//VAO
	GLuint VertexBufferObject;//VBO
	glGenVertexArrays(1,&VertexArrayObject);//? <----| keep in this order yt guy said it's important
	glGenBuffers(1,&VertexBufferObject);//?<---------|
	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//+ draw
	glDrawArrays(GL_TRIANGLES,0,6);
	//+ clean up
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VertexArrayObject);
	glDeleteBuffers(1, &VertexBufferObject);
}

static float frame = 0.0;
static float frame_step = 0.1;
static float ftime = 0;
static void RenderAll(std::vector<Object*>& Objects)
{
	ftime++;
	GLuint shaderFrame = glGetUniformLocation(LocalShader.ShaderProgram,"frame");
	GLuint shaderTime = glGetUniformLocation(LocalShader.ShaderProgram,"iTime");
	glUniform1f(shaderFrame,(float)frame);
	glUniform1f(shaderTime,ftime);
	frame+=frame_step;
	if(frame >= 1.0 || frame <= -1.0)
		frame_step *= -1;
	//for(uint64_t i = 0;i<Objects.size();i++)
	//{
	//	Object* obj = Objects[i];
	//	__drawSqaureTexture(obj->Pos,obj->Size,"./assets/Textures/background.png");
	//}
	drawSqaure(Objects[0]->Pos,Objects[0]->Size);
}

static void LoopAll(std::vector<Object*>& Objects)
{
}


static void PushObject(Object* obj)
{
}

__attribute__((constructor)) static void ctor()
{
	engine::PushObjectClass(&localClass);
}
