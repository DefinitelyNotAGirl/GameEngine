#define ENGINE_BACKEND
#include <Engine.hxx>
#include <iostream>
namespace engine
{
	keyCallbackT keyUpCallbacks  [384] = {nullptr};
	keyCallbackT keyDownCallbacks[384] = {nullptr};
	void setKeyUpCallback(int key,keyCallbackT function)
	{
		keyUpCallbacks[key] = function;
	}
	void setKeyDownCallback(int key,keyCallbackT function)
	{
		keyDownCallbacks[key] = function;
	}

	void globalKeyCallback(GLFWwindow* gwindow, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_UNKNOWN)return;
		if(action == GLFW_REPEAT)return;
		if(action == GLFW_PRESS)
		{
			if(keyDownCallbacks[key] == nullptr)return;
			keyDownCallbacks[key]();
		}
		else if(action == GLFW_RELEASE)
		{
			if(keyUpCallbacks[key] == nullptr)return;
			keyUpCallbacks[key]();
		}
	}

	void initKeyCallbacks(window* win)
	{
		glfwSetKeyCallback((GLFWwindow*)win->__GLFWwindow,&globalKeyCallback);
	}
}