#include <libCargparse.hxx>
#define ENGINE_BACKEND
#include <Engine.hxx>
#include <csignal>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

void cargHandler_unknown(CARGPARSE_UNKOWNHANDLER_ARGS) {
}

void sighandler(int sig)
{
	void *array[10];
	size_t size;
	// get void*'s for all entries on the stack
	size = backtrace(array, 10);
	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

using engine::window;
using engine::Position2D;
using engine::Size2D;
using engine::rgba;

#include <chrono>

//666667 ==> 1500 GU screen width
inline unsigned long long operator""GU(unsigned long long x){return x*666667;}
inline unsigned long long operator""BM(unsigned long long x){return x*31250000ULL;}

void game_main();
void game_loop();

using engine::ObjectClass;

using namespace engine;

namespace engine
{
	void computeLighting(Object* obj);
}

static void textShaderOnLoad(Shader* shader, uint64_t zplane)
{
	GLuint AtlasUni = glGetUniformLocation(shader->ShaderProgram,"FontAtlas");
	GLuint TextColorUni = glGetUniformLocation(shader->ShaderProgram,"textColor");
	glUniform1i(AtlasUni,2);
	glUniform4f(TextColorUni,0.5,0.0,1.0,1.0);
}

float fpsSize = 0.05;

static void renderCharTexture(uint8_t xpos,uint8_t ypos, float x, float y)
{
	//std::cout << "text render: " << (int)xpos << " " << (int)ypos << " " << x << " " << y << std::endl;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	float tfs = 0.03125;//1/32
	float tfx = tfs*xpos;
	float tfy = tfs*ypos;
	GLfloat vertices[] =
	{
		/* coords */x			,y			, 0.0f,		/* Texture mapping */tfx	, tfy		,/* lower left */
		/* coords */x+fpsSize	,y			, 0.0f,		/* Texture mapping */tfx+tfs, tfy		,/* lower right */
		/* coords */x+fpsSize	,y+fpsSize	, 0.0f,		/* Texture mapping */tfx+tfs, tfy+tfs	,/* upper right */
		/* coords */x+fpsSize	,y+fpsSize	, 0.0f,		/* Texture mapping */tfx+tfs, tfy+tfs	,/* upper right */
		/* coords */x			,y+fpsSize	, 0.0f,		/* Texture mapping */tfx	, tfy+tfs	,/* upper left */
		/* coords */x			,y			, 0.0f,		/* Texture mapping */tfx	, tfy		,/* lower left */
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

static void renderDigit(char c,float x, float y)
{
	renderCharTexture(c-'0',31,x,y);
}

Shader textShader(
	"./assets/shaders/Engine/text.vert",
	"./assets/shaders/Engine/text.frag",
	"text shader",
	&textShaderOnLoad
);

namespace engine
{
	extern uint64_t zplaneCount;
}
int main(int argc, char** argv)
{
	srand(static_cast <unsigned> (time(0)));
	uint64_t fps = 0;
	signal(11,&sighandler);
	signal(3,&sighandler);
	cargparse::instance carg;
	carg.unknownHandler = &cargHandler_unknown;
	carg.parse(argc,argv);
	//,
	//, initiate engine
	//,
	{
		engine::init();
	}
	//,
	//, execute game
	//,
	{
		std::cout << "running game..." << std::endl;
		game_main();
		engine::simpleLoadTexture("./assets/Textures/FontAtlas.png",GL_TEXTURE2);
		uint64_t fpsInterval = 0;
		while(1)
		{
			auto start = std::chrono::high_resolution_clock::now();
			engine::currentWindow->clearScreen(rgba(0x00,0x00,0x00,0xFF));
			glEnable(GL_MULTISAMPLE);
			try {
				//,
				//, draw stuff here
				//,
				GLuint currentShader = NULL;
				for(uint64_t I = 0;I<zplaneCount;I++)
				{
					//std::cout << "rendering zplane: " << I << "..." << std::endl;
					for(ObjectClass* c : engine::ObjectClasses)
					{
						std::vector<Object*>& zplane = c->Objects[I];
						if(zplane.size() == 0)
							continue;
						if(c->shader->ShaderProgram == NULL)
							c->shader->compile();
						if(c->shader->ShaderProgram != currentShader || true)
						{
							currentShader = c->shader->ShaderProgram;
							c->shader->activate();
							if(c->shader->onLoad != nullptr)
								c->shader->onLoad(c->shader,I);
						}
						c->RenderAll(zplane,I);
							//. this looks confusing i know, dont worry about it
					}
					//std::cout << "OK" << std::endl;
				}
				//,
				//, render FPS counter here
				//,
				{
					if(textShader.ShaderProgram == NULL)
						textShader.compile(false);
					textShader.activate();
					textShader.onLoad(&textShader,0);
					std::string sfps = std::to_string(fps);
					//std::cout << "sfps: " << sfps << std::endl;
					float posy = 1.0-fpsSize;
					float posx = 1.0-(fpsSize*(sfps.size()+3));
					for(char c : sfps)
					{
						renderDigit(c,posx,posy);
						posx+=fpsSize;
					}
					renderCharTexture(0,30,posx+(fpsSize*0),posy);
					renderCharTexture(1,30,posx+(fpsSize*1),posy);
					renderCharTexture(2,30,posx+(fpsSize*2),posy);
				}
			} catch(engine::ShaderLinkException e) {
				std::cerr << e.what << ": " << e.shader->name << std::endl;
				for(std::string src : e.shader->sourceFiles)
					std::cerr << "    " << src << std::endl;
				std::cerr << "logs: " << std::endl << e.logs << std::endl;
				exit(-1);
			}
			//,
			//, update screen
			//,
			{
				engine::currentWindow->updateScreen();
			}
			//,
			//, game loop
			//,
			{
				game_loop();
				//+
				//+ colissions
				//+
				//std::cout << "########################################" << std::endl;
				//std::cout << "########################################" << std::endl;
				//std::cout << "########################################" << std::endl;
				//std::cout << "           Compute Colissions"			<< std::endl;
				//std::cout << "########################################" << std::endl;
				//std::cout << "########################################" << std::endl;
				//std::cout << "########################################" << std::endl;
				{
					for(uint64_t I = 0;I<zplaneCount;I++)
					{
						//std::cout << "########################################" << std::endl;
						//std::cout << "########################################" << std::endl;
						//std::cout << "           z-plane: "<<I					<< std::endl;
						//std::cout << "########################################" << std::endl;
						//std::cout << "########################################" << std::endl;
						for(ObjectClass* c : engine::ObjectClasses)
						{
							if(!c->computePhysics)continue;
							std::vector<Object*>& zplane = c->Objects[I];
							if(zplane.size() == 0)continue;
							//std::cout << "########################################" << std::endl;
							//std::cout << "      class: "<<c->Name					<< std::endl;
							//std::cout << "########################################" << std::endl;
							for(Object* obj : zplane)
							{
								for(ObjectClass* compareClass : engine::ObjectClasses)
								{
									std::vector<Object*>& compareZplane = compareClass->Objects[I];
									if(compareZplane.size() == 0)continue;
									for(Object* compareObj : compareZplane)
									{
										if(compareObj == obj)continue;
										if(
											(
												//x check
												(
													(
														(obj->Pos.x) >= (compareObj->Pos.x) 
													)
													&&
													(
														(obj->Pos.x) <= (compareObj->Pos.x+compareObj->Hitbox.width)
													)
												)
												||
												(
													(
														(compareObj->Pos.x) >= (obj->Pos.x) 
													)
													&&
													(
														(compareObj->Pos.x) <= (obj->Pos.x+obj->Hitbox.width)
													)
												)
											)
											&&
											(
												//y check
												(
													(
														(obj->Pos.y) >= (compareObj->Pos.y) 
													)
													&&
													(
														(obj->Pos.y) <= (compareObj->Pos.y+compareObj->Hitbox.width)
													)
												)
												||
												(
													(
														(compareObj->Pos.y) >= (obj->Pos.y)
													)
													&&
													(
														(compareObj->Pos.y) <= (obj->Pos.y+obj->Hitbox.width)
													)
												)
											)
										)
										{
											obj->oclass->ColissionCallback(obj,compareObj);
										}
									}
								}
							}
						}
						//std::cout << "OK" << std::endl;
					}
				}
				for(ObjectClass* c : engine::ObjectClasses)for(std::vector<Object*> zplane : c->Objects)
					c->LoopAll(zplane);
			}
			//,
			//, poll input and act on it
			//,
			{
				glfwPollEvents();
			}
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			//if(fpsInterval == 10)
			{
				fps = 1000000/duration.count();
				//fpsInterval = 0;
			}
			//fpsInterval++;
		}
	}
	//,
	//, exit the program
	//,
	{
		std::cout << "exit..." << std::endl;
		return 0;
	}
}
