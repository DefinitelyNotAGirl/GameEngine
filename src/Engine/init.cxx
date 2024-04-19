#include <Engine/platform.h>
#include <Engine/common.hxx>
#include <Engine/Except.hxx>
#ifdef ENGINE_RENDER_OPENGL
	#define GLAD_GL_IMPLEMENTATION
	#include <glad/glad.h>
	#define GLFW_INCLUDE_NONE
	#include <GLFW/glfw3.h>
#endif
#include <iostream>
#include <Engine/stb_image.h>

namespace engine {
	void LoadBaseShaders() {
		try {
			//,
			//, Basic 2D RGB Shader
			//,
			{
				shaders::BasicRGBShader2D = new Shader("Basic 2D RGB");
				shaders::BasicRGBShader2D->compile(
					"./assets/shaders/Engine/Basic.vert.glsl",
					"./assets/shaders/Engine/Basic.frag.glsl"
				);
			}
			//,
			//, Basic 2D Texture Shader
			//,
			{
				shaders::BasicTextureShader2D = new Shader("Basic 2D Texutre");
				shaders::BasicTextureShader2D->compile(
					"./assets/shaders/Engine/Texture.vert.glsl",
					"./assets/shaders/Engine/Texture.frag.glsl"
				);
			}
		} catch(ShaderLinkException e) {
			std::cerr << e.what << ": " << e.shader->name << std::endl;
			for(std::string src : e.shader->sourceFiles)
				std::cerr << "    " << src << std::endl;
			std::cerr << "logs: " << std::endl << e.logs << std::endl;
			exit(-1);
		}
	}
	void init()
	{
		std::cout << "initiating graphics..." << std::endl;
	#ifdef ENGINE_RENDER_OPENGL
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 8);
	#endif
		stbi_set_flip_vertically_on_load(true);
		std::cout << "OK." << std::endl;
	}
}