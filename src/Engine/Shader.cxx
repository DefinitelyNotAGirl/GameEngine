#include <Engine/platform.h>

//, ███████ ███    ██  ██████  ██ ███    ██ ███████
//, ██      ████   ██ ██       ██ ████   ██ ██
//, █████   ██ ██  ██ ██   ███ ██ ██ ██  ██ █████
//, ██      ██  ██ ██ ██    ██ ██ ██  ██ ██ ██
//, ███████ ██   ████  ██████  ██ ██   ████ ███████
#include <Engine/Options.hxx>
#include <Engine/Except.hxx>
#include <Engine/Shader.hxx>

//, ███████ ██    ██ ███████ ████████ ███████ ███    ███
//, ██       ██  ██  ██         ██    ██      ████  ████
//, ███████   ████   ███████    ██    █████   ██ ████ ██
//,      ██    ██         ██    ██    ██      ██  ██  ██
//, ███████    ██    ███████    ██    ███████ ██      ██
#include <iostream>

namespace engine
{
	//, ██    ██ ████████ ██ ██
	//, ██    ██    ██    ██ ██
	//, ██    ██    ██    ██ ██
	//, ██    ██    ██    ██ ██
	//,  ██████     ██    ██ ███████
	static std::string GetShaderInfoLog(GLuint shader) {
		GLint TotalLenght=0;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&TotalLenght);
		std::string logs;
		logs.resize(TotalLenght);
		glGetShaderInfoLog(shader, TotalLenght, NULL, &logs[0] );
		return logs;
	}
	static std::string GetProgramInfoLog(GLuint program) {
		GLint TotalLenght=0;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&TotalLenght);
		std::string logs;
		logs.resize(TotalLenght);
		glGetProgramInfoLog(program, TotalLenght, NULL, &logs[0] );
		return logs;
	}
	static bool checkCompileSuccessful(GLuint shader) {
		int status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		return status == GL_TRUE;
	}
	static bool checkLinkSuccessful(GLuint program) {
		int status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		return status == GL_TRUE;
	}
	static char* ReadFileContent(char* file)
	{
		FILE* f = fopen(file,"r");
		if(f == nullptr)
			throw FileOpenException("Failed to open file",file);
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
		char* data = (char*)malloc(fsize + 1);
		uint64_t out = fread(data, fsize, 1, f);
		if(out != 1)
			throw FileReadException("Failed to read file",file,0,1,out,fsize);
		data[fsize] = 0;
		fclose(f);
		return data;
	}
	//, ███████ ██   ██  █████  ██████  ███████ ██████       ██████ ██       █████  ███████ ███████
	//, ██      ██   ██ ██   ██ ██   ██ ██      ██   ██     ██      ██      ██   ██ ██      ██
	//, ███████ ███████ ███████ ██   ██ █████   ██████      ██      ██      ███████ ███████ ███████
	//,      ██ ██   ██ ██   ██ ██   ██ ██      ██   ██     ██      ██      ██   ██      ██      ██
	//, ███████ ██   ██ ██   ██ ██████  ███████ ██   ██      ██████ ███████ ██   ██ ███████ ███████
	Shader::Shader(){}
	Shader::Shader(char* name)
		:name(name){}

	Shader::Shader(char* vertFile,char* fragFile,char* name,void(*onLoad)(Shader* shader))
		:name(name),vertFile(vertFile),fragFile(fragFile),onLoad(onLoad){}

	void Shader::compile()
	{
		this->compile(this->vertFile,this->fragFile);
	}

	void Shader::compile(char* vertFile,char* fragFile)
	{
		if(this->ShaderProgram != NULL)return;
		if(options::ddebug)std::cout << "Compiling Shader: " << this->name << std::endl;
		this->sourceFiles.push_back(std::string(vertFile));
		this->sourceFiles.push_back(std::string(fragFile));
		//,
		//, get vertex shader
		//,
		GLuint vertexShader;
		{
			if(options::ddebug)std::cout << "    vertex shader: " << vertFile << std::endl;
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			char* source;
			try {
				source = ReadFileContent(vertFile);
			} catch(FileOpenException e) {
				throw FileOpenException("[ENGINE] Failed to open vertex shader source file!",e.file);
			} catch(FileReadException e) {
				throw FileReadException("[ENGINE] Failed to read vertex shader source file!",e.file,e.start,e.requested,e.received,e.size);
			}
			glShaderSource(vertexShader,1,&source,NULL);
			glCompileShader(vertexShader);
			free(source);
		}
		//,
		//, get fragment shader
		//,
		GLuint fragmentShader;
		{
			if(options::ddebug)std::cout << "    fragment shader: " << fragFile << std::endl;
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			char* source;
			try {
				source = ReadFileContent(fragFile);
			} catch(FileOpenException e) {
				throw FileOpenException("[ENGINE] Failed to open fragment shader source file!",e.file);
			} catch(FileReadException e) {
				throw FileReadException("[ENGINE] Failed to read fragment shader source file!",e.file,e.start,e.requested,e.received,e.size);
			}
			glShaderSource(fragmentShader,1,&source,NULL);
			glCompileShader(fragmentShader);
			free(source);
		}
		//,
		//, get shader program
		//,
		{
			if(options::ddebug)std::cout << "    Linking...";
			GLuint shaderProgram = glCreateProgram();
			this->ShaderProgram = shaderProgram;
			//+
			//+ link
			//+
			{
				glAttachShader(shaderProgram,vertexShader);
				glAttachShader(shaderProgram,fragmentShader);
				glLinkProgram(shaderProgram);
			}
			//+
			//+ delete shaders
			//+
			{
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
			}
			//+
			//+ error checking
			//+
			{
				if(checkLinkSuccessful(shaderProgram) == false)
				{
					std::cout << std::endl;
					throw ShaderLinkException("[ENGINE] Failed to link shader",this,GetProgramInfoLog(shaderProgram));
				}
			}
			if(options::ddebug)std::cout << "OK" << std::endl;
		}
	}
	void Shader::activate()
	{
		glUseProgram(this->ShaderProgram);
	}
	//, ███████ ██   ██  █████  ██████  ███████ ██████       ██████  ██████       ██ ███████  ██████ ████████ ███████
	//, ██      ██   ██ ██   ██ ██   ██ ██      ██   ██     ██    ██ ██   ██      ██ ██      ██         ██    ██
	//, ███████ ███████ ███████ ██   ██ █████   ██████      ██    ██ ██████       ██ █████   ██         ██    ███████
	//,      ██ ██   ██ ██   ██ ██   ██ ██      ██   ██     ██    ██ ██   ██ ██   ██ ██      ██         ██         ██
	//, ███████ ██   ██ ██   ██ ██████  ███████ ██   ██      ██████  ██████   █████  ███████  ██████    ██    ███████
	namespace shaders
	{
		Shader* BasicRGBShader2D;
		Shader* BasicTextureShader2D;
	}
}
