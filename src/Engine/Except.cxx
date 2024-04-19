#include <Engine/Except.hxx>
#include <Engine/Window.hxx>

namespace engine
{
	GenericException::GenericException(char* what)
		:what(what){}

	BadCurrentWindowException::BadCurrentWindowException(window* currentWindow, char* what)
		:what(what),currentWindow(currentWindow){}

	FileOpenException::FileOpenException(char* what,char* file)
		:what(what),file(file){}

	FileWriteException::FileWriteException(char* what,char* file,uint64_t start,uint64_t sent,uint64_t written,uint64_t size)
		:what(what),file(file),start(start),size(size),sent(sent),written(written){}

	FileReadException::FileReadException(char* what,char* file,uint64_t start,uint64_t requested,uint64_t received,uint64_t size)
		:what(what),file(file),start(start),size(size),requested(requested),received(received){}

	ShaderLinkException::ShaderLinkException(char* what,Shader* shader, std::string logs)
		:what(what),shader(shader),logs(logs){}
}
