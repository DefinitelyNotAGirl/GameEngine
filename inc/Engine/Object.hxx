#pragma once

#include "platform.h"
//stdint & co.
#include "common.hxx"
//Engine
#include "Position.hxx"
#include "Size.hxx"
#include "RGBA.hxx"
#include "segregate.h"
#include "Shader.hxx"
#include "Lighting.hxx"
#include "Physics.hxx"
//system

namespace engine
{
	class ObjectClass;

	class Object {
	public:
		Position2D Pos;
		Size2D Size;
		Size2D Hitbox;
		uint64_t zplane;
		LightingProperties_T* LightingProperties;
		PhysicsProperties_T* PhysicsProperties;
		void* classData;//class specific data
		ObjectClass* oclass;
		Object(
			Position2D Pos,
			Size2D Size,
			Size2D Hitbox,
			uint64_t zplane,
			void* classData,
			ObjectClass* oclass
		);
	};
}
