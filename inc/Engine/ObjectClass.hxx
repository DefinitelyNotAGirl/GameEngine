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
#include "Physics.hxx"
//system

namespace engine
{
	typedef void(*GenericVoidFunction)();
	typedef void(*ObjectClassRenderIterator)(std::vector<Object*>& Objects,uint64_t zplane);
	typedef void(*ObjectClassLogicIterator)(std::vector<Object*>& Objects);
	typedef void(*PushObjectFunction)(Object* obj);
	typedef void(*ColissionCallback_T)(Object* obj, Object* colider);
	typedef char* ClassNotFoundException;
	class ObjectClass {
	public:
		char* Name;
		char* Texture;
		Shader* shader;
		LightingProperties_T* LightingProperties;
		PhysicsProperties_T* PhysicsProperties;
		bool computePhysics;// controls wether or not physics are to be computed for objects of this class
		ColissionCallback_T ColissionCallback;
		bool computeLighting;// controls wether or not the classes individual objects are to be tested for light computations
		std::vector< std::vector<Object*> > Objects;
			//. each top level vector represents 1 zplane
		ObjectClassRenderIterator RenderAll;
		ObjectClassLogicIterator LoopAll;
		PushObjectFunction Push;
		ObjectClass(
			char* Name,
			char* Texture,
			Shader* shader,
			LightingProperties_T* LightingProperties,
			PhysicsProperties_T* PhysicsProperties,
			bool computePhysics,
			ColissionCallback_T ColissionCallback,
			bool computeLighting,
			ObjectClassRenderIterator renderAll,
			ObjectClassLogicIterator loopAll,
			PushObjectFunction push
		);
		void PushGeneric(Object* obj);
	};
	extern std::vector<ObjectClass*> ObjectClasses;
}
