#pragma once

#include "platform.h"
//stdint & co.
#include "common.hxx"
//Engine
#include "Position.hxx"
#include "Size.hxx"
#include "RGBA.hxx"
#include "segregate.h"
//system

namespace engine
{
	class window {
	backend:
		uint64_t Width;
		uint64_t Height;
		void* Framebuffer;
	#ifdef ENGINE_RENDER_LINUX
		uint64_t XWindowID;
		void* XDisplay;
	#endif
	#ifdef ENGINE_RENDER_OPENGL
		void* __GLFWwindow;
	#endif
	public:
		/**
		 * @brief creates a new window and opens it
		 * @param initialWindowPosition the position at which the window will initially appear
		 * @param size the size of the window
		 * @param Title the text to show in the windows title bar
		 * @throws throws engine::GenericException if window can't be created
		*/
		window(Position2D initialWindowPosition,Size2D size,const char* Title);
		/**
		 * @brief sets all pixels on the screen to <color>
		 * @param color example: rgba(0,0,0,255) == opaque black
		*/
		void clearScreen(rgba color);
		/**
		 * @brief makes this window the current window used for drawing
		*/
		void makeCurrent();
		/**
		 * @brief updates the screen, things drawn to the screen won't show until this function is called
		*/
		void updateScreen();
	backend:
		/**
		 * @brief returns the frame buffer address
		*/
		void* GetFrameBuffer();
	};
	#ifndef ENGINE_FRONTEND
		extern window* currentWindow;
	#endif
}