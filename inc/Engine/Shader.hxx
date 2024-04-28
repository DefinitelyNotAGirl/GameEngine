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
	class Shader;
	typedef void(*shaderOnLoad)(Shader* shader,uint64_t zplane);
	class Shader {
	backend:
		std::vector<std::string> sourceFiles;
		std::string name;
		char* vertFile;//may be nullptr
		char* fragFile;//may be nullptr
		GLuint ShaderProgram = NULL;
	public:
		Shader(char* name);
		Shader(char* vertFile,char* fragFile,char* name,shaderOnLoad onLoad);
		Shader();
		void compile(bool useDefaultShaders = true);
		void compile(char* vertFile,char* fragFile,bool useDefaultShaders = true);
		void activate();
		shaderOnLoad onLoad = nullptr;
	};

	namespace shaders
	{
		extern Shader* BasicRGBShader2D;
		extern Shader* BasicTextureShader2D;
	}
}
