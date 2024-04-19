#pragma once

#include "platform.h"
#include "segregate.h"
//stdint & co.
#include "common.hxx"
//Engine
//system
#include <string>
#include <vector>

namespace engine
{
	class Shader {
	backend:
		std::vector<std::string> sourceFiles;
		std::string name;
		char* vertFile;//may be nullptr
		char* fragFile;//may be nullptr
		GLuint ShaderProgram = NULL;
	public:
		Shader(char* name);
		Shader(char* vertFile,char* fragFile,char* name,void(*onLoad)(Shader* shader));
		Shader();
		void compile();
		void compile(char* vertFile,char* fragFile);
		void activate();
		void (*onLoad)(Shader* shader) = nullptr;
	};

	namespace shaders
	{
		extern Shader* BasicRGBShader2D;
		extern Shader* BasicTextureShader2D;
	}
}
