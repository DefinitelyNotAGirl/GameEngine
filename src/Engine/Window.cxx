#define ENGINE_BACKEND
#include <Engine/platform.h>

//, ███████ ███    ██  ██████  ██ ███    ██ ███████
//, ██      ████   ██ ██       ██ ████   ██ ██
//, █████   ██ ██  ██ ██   ███ ██ ██ ██  ██ █████
//, ██      ██  ██ ██ ██    ██ ██ ██  ██ ██ ██
//, ███████ ██   ████  ██████  ██ ██   ████ ███████
#include <Engine/Window.hxx>
#include <Engine/Except.hxx>
#include <Engine.hxx>

//, ███████ ██    ██ ███████ ████████ ███████ ███    ███
//, ██       ██  ██  ██         ██    ██      ████  ████
//, ███████   ████   ███████    ██    █████   ██ ████ ██
//,      ██    ██         ██    ██    ██      ██  ██  ██
//, ███████    ██    ███████    ██    ███████ ██      ██
#include <iostream>

namespace engine
{
	window::window(Position2D initialWindowPosition,Size2D size,const char* Title)
		:Width(size.width),Height(size.height)
	{
	#ifdef ENGINE_RENDER_OPENGL
		std::cout << "[ENGINE] creating window...";
		GLFWwindow* __window = glfwCreateWindow(size.width,size.height,Title,NULL,NULL);
		if(__window == nullptr)
			throw GenericException("[ENGINE] assertion failed: __window != nullptr");
		this->__GLFWwindow = (void*)__window;
		glfwShowWindow(__window);
		//int w,h;
		//int fbw,fbh;
		//glfwGetWindowSize(__window,&w,&h);
		//glfwGetFramebufferSize(__window,&fbw,&fbh);
		std::cout << "OK." << std::endl;
		//std::cout << "window size: " << w << "x" << h << std::endl;
		//std::cout << "framebuffer size: " << fbw << "x" << fbh << std::endl;
	#endif
	}

	void window::updateScreen()
	{
	#ifdef ENGINE_RENDER_OPENGL
		glfwSwapBuffers((GLFWwindow*)this->__GLFWwindow);
	#endif
	}

	void window::makeCurrent()
	{
	#ifdef ENGINE_RENDER_OPENGL
		glfwMakeContextCurrent((GLFWwindow*)this->__GLFWwindow);
		gladLoadGL();
		glViewport(0,0,this->Width,this->Height);
		currentWindow = this;
		//LoadBaseShaders();
	#endif
	}

	void window::clearScreen(rgba color)
	{
	#ifdef ENGINE_RENDER_OPENGL
		glClearColor(color.Red,color.Green,color.Blue,color.Alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	#endif
	}

	window* currentWindow = nullptr;
}

//!  ██████  ██████  ██████  ███████     ██████  ██    ██ ███    ███ ██████
//! ██      ██    ██ ██   ██ ██          ██   ██ ██    ██ ████  ████ ██   ██
//! ██      ██    ██ ██   ██ █████       ██   ██ ██    ██ ██ ████ ██ ██████
//! ██      ██    ██ ██   ██ ██          ██   ██ ██    ██ ██  ██  ██ ██
//!  ██████  ██████  ██████  ███████     ██████   ██████  ██      ██ ██
#if false
#include <graphics.hxx>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* window;
uint64_t windowSizeH;
uint64_t windowSizeV;

char* blockTextureFile;

GLint mvp_location;
GLint vpos_location;
GLint vcol_location;

typedef struct Vertex
{
	vec2 pos;
	vec3 col;
} Vertex;

//, ███████ ██   ██  █████  ██████  ███████ ██████  ███████
//, ██      ██   ██ ██   ██ ██   ██ ██      ██   ██ ██
//, ███████ ███████ ███████ ██   ██ █████   ██████  ███████
//,      ██ ██   ██ ██   ██ ██   ██ ██      ██   ██      ██
//, ███████ ██   ██ ██   ██ ██████  ███████ ██   ██ ███████

namespace shaders
{
	GLuint s1;
	GLuint block;
	GLuint testShader;
}

char* loadFile(char* file)
{
	FILE* f = fopen(file,"r");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
	char* data = (char*)malloc(fsize + 1);
	fread(data, fsize, 1, f);
	data[fsize] = 0;
	fclose(f);
	return data;
}
std::string GetShaderInfoLog(GLuint shader) {
	GLint TotalLenght=0;
	glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&TotalLenght);
	std::string logs;
	logs.resize(TotalLenght);
	glGetShaderInfoLog(shader, TotalLenght, NULL, &logs[0] );
	return logs;
}
std::string GetProgramInfoLog(GLuint program) {
	GLint TotalLenght=0;
	glGetProgramiv(program,GL_INFO_LOG_LENGTH,&TotalLenght);
	std::string logs;
	logs.resize(TotalLenght);
	glGetProgramInfoLog(program, TotalLenght, NULL, &logs[0] );
	return logs;
}
bool checkCompileSuccessful(GLuint shader) {
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;
}
bool checkLinkSuccessful(GLuint program) {
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	return status == GL_TRUE;
}
GLuint loadShader(char* fragFile,char* vertFile)
{
	const GLuint program = glCreateProgram();
	//,
	//, vertex
	//,
	{
		char* source = loadFile(vertFile);
		const GLuint shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		glAttachShader(program, shader);
		if(!checkCompileSuccessful(shader))
		{
			std::cerr << "shader ("<<vertFile<<") logs:\n"<< GetShaderInfoLog(shader) <<  std::endl;
		}
		free(source);
	}
	//,
	//, fragment
	//,
	{
		char* source = loadFile(fragFile);
		const GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		glAttachShader(program, shader);
		if(!checkCompileSuccessful(shader))
		{
			std::cerr << "shader ("<<fragFile<<") logs:\n"<< GetShaderInfoLog(shader) <<  std::endl;
		}
		free(source);
	}
	glLinkProgram(program);
	if(!checkLinkSuccessful(program))
	{
		std::cerr << "error: could not link shader program!" << std::endl;
		std::cerr << "program logs:\n"<< GetProgramInfoLog(program) <<  std::endl;
	}
	return program;
}

//, ██ ███    ██ ██ ████████
//, ██ ████   ██ ██    ██
//, ██ ██ ██  ██ ██    ██
//, ██ ██  ██ ██ ██    ██
//, ██ ██   ████ ██    ██
#define shaderPath "shaders"
void graphics_init()
{
}
//,  ██████ ██████  ███████  █████  ████████ ███████     ██     ██ ██ ███    ██ ██████   ██████  ██     ██
//, ██      ██   ██ ██      ██   ██    ██    ██          ██     ██ ██ ████   ██ ██   ██ ██    ██ ██     ██
//, ██      ██████  █████   ███████    ██    █████       ██  █  ██ ██ ██ ██  ██ ██   ██ ██    ██ ██  █  ██
//, ██      ██   ██ ██      ██   ██    ██    ██          ██ ███ ██ ██ ██  ██ ██ ██   ██ ██    ██ ██ ███ ██
//,  ██████ ██   ██ ███████ ██   ██    ██    ███████      ███ ███  ██ ██   ████ ██████   ██████   ███ ███
void createWindow(uint64_t width,uint64_t height)
{
	window = glfwCreateWindow(width,height,"Tetris",NULL,NULL);
	if(window == nullptr)
	{
		std::cerr << "ERROR: could not create window!" << std::endl;
		exit(-1);
	}
	windowSizeH = width;
	windowSizeV = height;
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwShowWindow(window);
	//,
	//, load shaders
	//,
	{
		shaders::s1 = loadShader(
			shaderPath"/frag.glsl",
			shaderPath"/vert.glsl"
		);
		shaders::block = loadShader(
			shaderPath"/block.frag.glsl",
			shaderPath"/block.vert.glsl"
		);
		shaders::testShader = loadShader(
			shaderPath"/test.frag",
			shaderPath"/test.vert"
		);
	}
	glUseProgram(shaders::s1);
	mvp_location = glGetUniformLocation(shaders::s1, "MVP");
    vpos_location = glGetAttribLocation(shaders::s1, "vPos");
    vcol_location = glGetAttribLocation(shaders::s1, "vCol");
}
//, ██    ██ ██████  ██████   █████  ████████ ███████
//, ██    ██ ██   ██ ██   ██ ██   ██    ██    ██
//, ██    ██ ██████  ██   ██ ███████    ██    █████
//, ██    ██ ██      ██   ██ ██   ██    ██    ██
//,  ██████  ██      ██████  ██   ██    ██    ███████
void updateScreen()
{
	glfwSwapBuffers(window);
    glfwPollEvents();
	glViewport(0, 0, windowSizeH, windowSizeV);
	glClear(GL_COLOR_BUFFER_BIT);
}
//,  ██████ ██       ██████  ███████ ███████
//, ██      ██      ██    ██ ██      ██
//, ██      ██      ██    ██ ███████ █████
//, ██      ██      ██    ██      ██ ██
//,  ██████ ███████  ██████  ███████ ███████
bool windowShouldClose()
{
	return glfwWindowShouldClose(window);
}
//,######################################################################################################
//,######################################################################################################
//, ██████  ██████   █████  ██     ██     ██████  ███████  ██████ ████████
//, ██   ██ ██   ██ ██   ██ ██     ██     ██   ██ ██      ██         ██
//, ██   ██ ██████  ███████ ██  █  ██     ██████  █████   ██         ██
//, ██   ██ ██   ██ ██   ██ ██ ███ ██     ██   ██ ██      ██         ██
//, ██████  ██   ██ ██   ██  ███ ███      ██   ██ ███████  ██████    ██
//,######################################################################################################
//,######################################################################################################
void drawRect(Vertex rectv[4],vec3 rgb)
{
	//,
	//, one piece rect
	//,
	{
		const Vertex vertices[6] =
		{
			/* top right */rectv[1],
			/* bottom left */rectv[2],
			/* bottom right */rectv[3],
			/* top left */rectv[0],
			/* top right */rectv[1],
			/* bottom left */rectv[2],
		};
		GLuint vertex_buffer;
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		GLuint vertex_array;
		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
		                      sizeof(Vertex), (void*) offsetof(Vertex, pos));
		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
		                      sizeof(Vertex), (void*) offsetof(Vertex, col));
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		const float ratio = width / (float) height;
		mat4x4 m, p, mvp;
		mat4x4_identity(m);
		//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
		glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
//,######################################################################################################
//,######################################################################################################
//, ██████  ██████   █████  ██     ██     ████████ ███████ ██   ██ ████████ ██    ██ ██████  ███████
//, ██   ██ ██   ██ ██   ██ ██     ██        ██    ██       ██ ██     ██    ██    ██ ██   ██ ██
//, ██   ██ ██████  ███████ ██  █  ██        ██    █████     ███      ██    ██    ██ ██████  █████
//, ██   ██ ██   ██ ██   ██ ██ ███ ██        ██    ██       ██ ██     ██    ██    ██ ██   ██ ██
//, ██████  ██   ██ ██   ██  ███ ███         ██    ███████ ██   ██    ██     ██████  ██   ██ ███████
//,######################################################################################################
//,######################################################################################################
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
void drawTexture()
{
	int width,height,channels = 0;
	char* data = (char*)stbi_load("block.png",&width,&height,&channels,2);
	if(data == nullptr)
	{
		std::cerr << "failed to load texture" << std::endl;
		return;
	}

	GLuint textureID;
	glGenTextures(1,&textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float edgeColor[] = {1.0f,1.0f,0.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, edgeColor);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D,0);

	GLfloat vertices[] = 
	{
		-0.5f, -0.5f,  0.0f,	 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, //lower left corner
		-0.5f,  0.5f,  0.0f,	 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //lower left corner
		 0.5f,  0.5f,  0.0f,	 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //lower left corner
		 0.5f, -0.5f,  0.0f,	 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //lower left corner
	};
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glDrawArrays(GL_QUADS, 0, 4);
}
//,######################################################################################################
//,######################################################################################################
//, ██████  ██████   █████  ██     ██     ██████  ██       ██████   ██████ ██   ██
//, ██   ██ ██   ██ ██   ██ ██     ██     ██   ██ ██      ██    ██ ██      ██  ██
//, ██   ██ ██████  ███████ ██  █  ██     ██████  ██      ██    ██ ██      █████
//, ██   ██ ██   ██ ██   ██ ██ ███ ██     ██   ██ ██      ██    ██ ██      ██  ██
//, ██████  ██   ██ ██   ██  ███ ███      ██████  ███████  ██████   ██████ ██   ██
//,#####################################################################################################
//,#####################################################################################################
#define BlocksOffsetH -0.50125f
#define BlocksOffsetV 1.0f
#define TetrisBlockSizeH 0.1f
#define TetrisBlockSizeV 0.1f
void drawTetrisBlock(vec3 outline, vec3 fill, uint64_t x, uint64_t y)
{
	//,
	//, draw texture
	//,
	if(false){
		GLuint tex0Uni = glGetUniformLocation(shaders::block,"tex0");
		glUseProgram(shaders::block);
		glUniform1i(tex0Uni,0);
		drawTexture();
	}
	//,
	//, block fill
	//,
	{
		glUseProgram(shaders::testShader);
		Vertex vertices[4] =
		{
			/* top left */
			{
				{
					BlocksOffsetH+(x*TetrisBlockSizeH),
					BlocksOffsetV-(y*TetrisBlockSizeV)
				},
				{fill[0],fill[1],fill[2]}
			},
			/* top right */
			{
				{
					BlocksOffsetH+(x*TetrisBlockSizeH)+TetrisBlockSizeH,
					BlocksOffsetV-(y*TetrisBlockSizeV)
				},
				{fill[0],fill[1],fill[2]}
			},
			/* bottom left */
			{
				{
					BlocksOffsetH+(x*TetrisBlockSizeH),
					BlocksOffsetV-(y*TetrisBlockSizeV)-TetrisBlockSizeV
				},
				{fill[0],fill[1],fill[2]}
			},
			/* bottom right */
			{
				{
					BlocksOffsetH+(x*TetrisBlockSizeH)+TetrisBlockSizeH,
					BlocksOffsetV-(y*TetrisBlockSizeV)-TetrisBlockSizeV
				},
				{fill[0],fill[1],fill[2]}
			}
		};
		drawRect(vertices,fill);
	}
}
#endif
