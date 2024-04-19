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
#include "Object.hxx"
#include "Lighting.hxx"
//system

namespace engine
{
	typedef void(*GenericVoidFunction)();
	typedef void(*ObjectClassIterator)(std::vector<Object*>& Objects);
	typedef void(*PushObjectFunction)(Object* obj);
	typedef char* ClassNotFoundException;
	class ObjectClass {
	public:
		char* Name;
		char* Texture;
		Shader* shader;
		LightingProperties_T* LightingProperties;
		std::vector<Object*> Objects;
		ObjectClassIterator RenderAll;
		ObjectClassIterator LoopAll;
		PushObjectFunction Push;
		ObjectClass(
			char* Name,
			char* Texture,
			Shader* shader,
			LightingProperties_T* LightingProperties,
			ObjectClassIterator renderAll,
			ObjectClassIterator loopAll,
			PushObjectFunction push
		);
		void PushGeneric(Object* obj);
	};
	extern std::vector<ObjectClass*> ObjectClasses;
}
