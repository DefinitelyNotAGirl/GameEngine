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

int main(int argc, char** argv)
{
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
		game_main();
		std::cout << "running game..." << std::endl;
		while(1)
		{
			auto start = std::chrono::high_resolution_clock::now();
			engine::currentWindow->clearScreen(rgba(0x00,0x00,0x00,0xFF));
			glEnable(GL_MULTISAMPLE);
			//,
			//, draw stuff here
			//,
			try {
				GLuint currentShader = NULL;
				for(ObjectClass* c : engine::ObjectClasses)
				{
					if(c->shader->ShaderProgram == NULL)
						c->shader->compile();
					if(c->shader->ShaderProgram != currentShader)
					{
						c->shader->activate();
						if(c->shader->onLoad != nullptr)
							c->shader->onLoad(c->shader);
					}
					c->RenderAll(c->Objects);
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
			//, poll input and act on it
			//,
			{
			}
			//,
			//, game loop
			//,
			{
				for(ObjectClass* c : engine::ObjectClasses)
					c->LoopAll(c->Objects);
				game_loop();
			}
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			fps = 1000000/duration.count();
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
