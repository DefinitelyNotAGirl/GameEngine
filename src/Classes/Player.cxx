#define ClassName "Class.Player"

#define ENGINE_BACKEND
#include <Engine.hxx>
#include <iostream>
using namespace engine;

static void PushObject(Object* obj);
static void LoopAll(std::vector<Object*>& Objects);
static void RenderAll(std::vector<Object*>& Objects, uint64_t zplane);
#ifndef ClassName
	#error please define a class name
#endif
#define LOCAL_CLASS_TRANSPARENCY 0.0f
#define LocalClassName ClassName
static LightingProperties_T LightingProperties(
	rgba(0.0f,1.0f,1.0f,0.0f),
	0.0,
	0.2,
	(1<<0)|(1<<1)|(1<<2),
	rgba(1.0f,0.2f,1.0f,LOCAL_CLASS_TRANSPARENCY),
	0.0,
	5.0f
);

static PhysicsProperties_T PhysicsProperties(
	1.0,
	1.0,
	0.0,
	0.0,
	1.0
);

#include <string.h>

static void __Shader_Player__onLoad(Shader* shader, uint64_t zplane);

extern int64_t velocityY;
void resetPlayerJumpAnimation();

void killPlayer();
extern bool playerDoGravity;
static void ColissionCallback(Object* player, Object* colider)
{
	velocityY = 0;
	resetPlayerJumpAnimation();
	if(colider->oclass->Name == "Class.Level.Lava")
		killPlayer();
	playerDoGravity = false;
	//std::cout << "player colission with object of type: \"" << colider->oclass->Name <<"\""<< std::endl;
}

static Shader __Shader_Player(
	"./assets/shaders/Player.vert",
	"./assets/shaders/Player.frag",
	"Player Shader",
	&__Shader_Player__onLoad
);
static ObjectClass localClass(
	LocalClassName,
	nullptr,
	&__Shader_Player,
	&LightingProperties,
	&PhysicsProperties,
	true,
	&ColissionCallback,
	true,
	&RenderAll,
	&LoopAll,
	&PushObject
);

static void __Shader_Player__onLoad(Shader* shader, uint64_t zplane)
{
	GLuint PlayerTextureUni = glGetUniformLocation(localClass.shader->ShaderProgram,"PlayerTexture");
	glUniform1i(PlayerTextureUni,1);
	computeLightingForShaderOnZPlane(shader,zplane);
}

uint8_t playerPhase = 0;
uint8_t playerPhaseInterval = 0;

extern uint8_t playerAnimY;
extern uint8_t playerAnimX;
extern bool flipPlayer;

static void drawPlayer(Position2D pos,Size2D size)
{
	if(currentWindow == nullptr)
		throw BadCurrentWindowException(currentWindow,"["LocalClassName"] assertion failed: currentWindow != nullptr");
	RenderPosition2D rp(pos);
	RenderSize2D rs(size);
	/*
	std::cout << "rp: " << rp.x << ":" << rp.y << std::endl;
	std::cout << "rs: " << rs.width << "x" << rs.height << std::endl;
	*/
	float tfsX = 0.142857142857143;//1/7
	float tfsY = 0.0909090909091;//1/11
	uint8_t xpos = playerPhase+playerAnimX;
	uint8_t ypos = playerAnimY;
	float tfx = tfsX*xpos;
	float tfy = tfsY*ypos;
	GLfloat vertices[30];
	if(flipPlayer == false)
	{
		GLfloat __vertices[] =
		{
			/* coords */rp.x			,rp.y			, 1.0f,		/* Texture mapping */tfx		, tfy		,/* lower left */
			/* coords */rp.x+rs.width	,rp.y			, 1.0f,		/* Texture mapping */tfx+tfsX	, tfy		,/* lower right */
			/* coords */rp.x+rs.width	,rp.y+rs.height	, 1.0f,		/* Texture mapping */tfx+tfsX	, tfy+tfsY	,/* upper right */
			/* coords */rp.x+rs.width	,rp.y+rs.height	, 1.0f,		/* Texture mapping */tfx+tfsX	, tfy+tfsY	,/* upper right */
			/* coords */rp.x			,rp.y+rs.height	, 1.0f,		/* Texture mapping */tfx		, tfy+tfsY	,/* upper left */
			/* coords */rp.x			,rp.y			, 1.0f,		/* Texture mapping */tfx		, tfy		,/* lower left */
		};
		memcpy(vertices,__vertices,sizeof(__vertices));
	}
	else
	{
		GLfloat __vertices[] =
		{
			/* coords */rp.x			,rp.y			, 1.0f,		/* Texture mapping */tfx+tfsX	, tfy		,/* lower left */
			/* coords */rp.x+rs.width	,rp.y			, 1.0f,		/* Texture mapping */tfx		, tfy		,/* lower right */
			/* coords */rp.x+rs.width	,rp.y+rs.height	, 1.0f,		/* Texture mapping */tfx		, tfy+tfsY	,/* upper right */
			/* coords */rp.x+rs.width	,rp.y+rs.height	, 1.0f,		/* Texture mapping */tfx		, tfy+tfsY	,/* upper right */
			/* coords */rp.x			,rp.y+rs.height	, 1.0f,		/* Texture mapping */tfx+tfsX	, tfy+tfsY	,/* upper left */
			/* coords */rp.x			,rp.y			, 1.0f,		/* Texture mapping */tfx+tfsX	, tfy		,/* lower left */
		};
		memcpy(vertices,__vertices,sizeof(__vertices));
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
}

static void RenderAll(std::vector<Object*>& Objects, uint64_t zplane)
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	for(uint64_t i = 0;i<Objects.size();i++)
	{
		Object* obj = Objects[i];
		drawPlayer(obj->Pos,obj->Size);
	}
	glDisable(GL_BLEND);
	playerPhaseInterval++;
	if(playerPhaseInterval == 15)
	{
		playerPhase++;
		if(playerPhase == 4)playerPhase = 0;
		playerPhaseInterval = 0;
	}
}

static void LoopAll(std::vector<Object*>& Objects)
{
}
static void PushObject(Object* obj){}
__attribute__((constructor)) static void ctor()
{
	engine::PushObjectClass(&localClass);
}
