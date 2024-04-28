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
	static void attachShader(GLuint program, char* file,GLenum type,std::vector<GLuint>& shaders)
	{
		if(options::ddebug)std::cout << "    " << file << std::endl;
		GLuint shader = glCreateShader(type);
		char* source;
		try {
			source = ReadFileContent(file);
		} catch(FileOpenException e) {
			throw FileOpenException("[ENGINE] Failed to open shader source file!",e.file);
		} catch(FileReadException e) {
			throw FileReadException("[ENGINE] Failed to read shader source file!",e.file,e.start,e.requested,e.received,e.size);
		}
		glShaderSource(shader,1,&source,NULL);
		glCompileShader(shader);
		glAttachShader(program,shader);
		shaders.push_back(shader);
		free(source);
	}
	//, ███████ ██   ██  █████  ██████  ███████ ██████       ██████ ██       █████  ███████ ███████
	//, ██      ██   ██ ██   ██ ██   ██ ██      ██   ██     ██      ██      ██   ██ ██      ██
	//, ███████ ███████ ███████ ██   ██ █████   ██████      ██      ██      ███████ ███████ ███████
	//,      ██ ██   ██ ██   ██ ██   ██ ██      ██   ██     ██      ██      ██   ██      ██      ██
	//, ███████ ██   ██ ██   ██ ██████  ███████ ██   ██      ██████ ███████ ██   ██ ███████ ███████
	Shader::Shader(){}
	Shader::Shader(char* name)
		:name(name){}

	Shader::Shader(char* vertFile,char* fragFile,char* name,shaderOnLoad onLoad)
		:name(name),vertFile(vertFile),fragFile(fragFile),onLoad(onLoad){}

	void Shader::compile(bool useDefaultShaders)
	{
		this->compile(this->vertFile,this->fragFile,useDefaultShaders);
	}

	void Shader::compile(char* vertFile,char* fragFile,bool useDefaultShaders)
	{
		if(this->ShaderProgram != NULL)return;
		if(options::ddebug)std::cout << "Compiling Shader: " << this->name << std::endl;
		this->sourceFiles.push_back(std::string(vertFile));
		this->sourceFiles.push_back(std::string(fragFile));
		if(useDefaultShaders)
		{
			this->sourceFiles.push_back(std::string("./assets/shaders/Engine/Lighting.frag"));
		}

		std::vector<GLuint> shaders;
		GLuint shaderProgram = glCreateProgram();
		//, attach shaders
		attachShader(shaderProgram,vertFile,GL_VERTEX_SHADER,shaders);
		attachShader(shaderProgram,fragFile,GL_FRAGMENT_SHADER,shaders);
		if(useDefaultShaders)
		{
			attachShader(shaderProgram,"./assets/shaders/Engine/Lighting.frag",GL_FRAGMENT_SHADER,shaders);
		}
		//, link
		if(options::ddebug)std::cout << "    Linking...";
		glLinkProgram(shaderProgram);
		if(checkLinkSuccessful(shaderProgram) == false)
		{
			std::cout << std::endl;
			throw ShaderLinkException("[ENGINE] Failed to link shader",this,GetProgramInfoLog(shaderProgram));
		}
		this->ShaderProgram = shaderProgram;
		if(options::ddebug)std::cout << "OK" << std::endl;
		//, delete shaders
		for(GLuint shader : shaders)
		{
			glDeleteShader(shader);
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
