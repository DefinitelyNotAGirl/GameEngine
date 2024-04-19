//, ██████  ██       █████  ████████ ███████  ██████  ██████  ███    ███
//, ██   ██ ██      ██   ██    ██    ██      ██    ██ ██   ██ ████  ████
//, ██████  ██      ███████    ██    █████   ██    ██ ██████  ██ ████ ██
//, ██      ██      ██   ██    ██    ██      ██    ██ ██   ██ ██  ██  ██
//, ██      ███████ ██   ██    ██    ██       ██████  ██   ██ ██      ██
#include <Engine/sys.h>
#define ENGINE_RENDER_OPENGL
#if defined(ENGINE_RENDER_OPENGL)
	#include <glad/glad.h>
	#define GLFW_INCLUDE_NONE
	#include <GLFW/glfw3.h>
#elif defined(ENGINE_RENDER_VULKAN)
#elif defined(ENGINE_LINUX)
	#define ENGINE_RENDER_LINUX
	#include <X11/Xlib.h>
	#include <X11/X.h>
#endif