#pragma once
#ifndef ENGINE_BACKEND
	#define ENGINE_FRONTEND
#endif

#include "Engine/Except.hxx"
#include "Engine/Window.hxx"
#include "Engine/ObjectClass.hxx"
namespace engine
{
	void LoadBaseShaders();
	void init();
	void PushObjectGlobal(Object* obj);
	void PushObjectClass(ObjectClass* c);
	ObjectClass* getObjectClass(char* classname);
	void computeLightingForShaderOnZPlane(Shader* shader,uint64_t zplane);
	void setZPlaneCount(uint64_t count);
	//, ██    ██ ███████ ███████ ██████      ██ ███    ██ ██████  ██    ██ ████████
	//, ██    ██ ██      ██      ██   ██     ██ ████   ██ ██   ██ ██    ██    ██
	//, ██    ██ ███████ █████   ██████      ██ ██ ██  ██ ██████  ██    ██    ██
	//, ██    ██      ██ ██      ██   ██     ██ ██  ██ ██ ██      ██    ██    ██
	//,  ██████  ███████ ███████ ██   ██     ██ ██   ████ ██       ██████     ██
	typedef void(*keyCallbackT)();
	void setKeyUpCallback(int key,keyCallbackT function);
	void setKeyDownCallback(int key,keyCallbackT function);
	void initKeyCallbacks(window* win);
	//, ███████ ██ ███    ███ ██████  ██      ███████
	//, ██      ██ ████  ████ ██   ██ ██      ██
	//, ███████ ██ ██ ████ ██ ██████  ██      █████
	//,      ██ ██ ██  ██  ██ ██      ██      ██
	//, ███████ ██ ██      ██ ██      ███████ ███████
	void simpleLoadTexture(char* file, GLenum slot);
	//, ██████  ██████   █████  ██     ██
	//, ██   ██ ██   ██ ██   ██ ██     ██
	//, ██   ██ ██████  ███████ ██  █  ██
	//, ██   ██ ██   ██ ██   ██ ██ ███ ██
	//, ██████  ██   ██ ██   ██  ███ ███
	/**
	 * @brief draw a triangle using its 3 corners
	 * @param corners an array of 3 Position2D objects containing 1 corner each
	 * @param color the color to draw the triangle as
	 * @param defaultShader if this parameter is true then the shader will be changed to some default shader
	 * @throws Throws a BadCurrentWindowException if the current window can't be drawn on
	*/
	void drawTriangle(Position2D corners[3],rgba color,bool defaultShader = false);
	/**
	 * @brief draw a sqaure texture
	 * @param pos the position of the textures upper left corner
	 * @param size the size
	 * @param file file from which to load texture
	 * @param defaultShader if this parameter is true then the shader will be changed to some default shader
	 * @throws Throws a BadCurrentWindowException if the current window can't be drawn on
	*/
	void drawSqaureTexture(Position2D pos,Size2D size, char* file,bool defaultShader = false);
}
