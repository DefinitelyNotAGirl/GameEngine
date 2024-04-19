#pragma once

#include "common.hxx"
#include <vector>
#include <string>
#include "Shader.hxx"
#include "Window.hxx"

namespace engine
{
	class GenericException {
	public:
		char* what;
		GenericException(char* what);
	};

	class BadCurrentWindowException {
	public:
		char* what;
		window* currentWindow;
		BadCurrentWindowException(window* currentWindow,char* what);
	};

	class FileOpenException {
	public:
		char* what;
		char* file;
		FileOpenException(char* what,char* file);
	};

	class FileReadException {
	public:
		char* what;
		char* file;
		uint64_t start;/* start byte of operation */
		uint64_t requested;/* number of items requested */
		uint64_t received;/* number of items received */
		uint64_t size;/* size of each item */
		FileReadException(char* what,char* file,uint64_t start,uint64_t requested,uint64_t received,uint64_t size);
	};

	class FileWriteException {
	public:
		char* what;
		char* file;
		uint64_t start;/* start byte of operation */
		uint64_t sent;/* number of items that were supposed to be written */
		uint64_t written;/* number of items actually written */
		uint64_t size;/* size of each item */
		FileWriteException(char* what,char* file,uint64_t start,uint64_t sent,uint64_t written,uint64_t size);
	};

	class ShaderLinkException {
	public:
		char* what;
		Shader* shader;
		std::string logs;
		ShaderLinkException(char* what,Shader* shader, std::string logs);
	};
}