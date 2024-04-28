#define ENGINE_BACKEND
#include <Engine.hxx>
#include <iostream>
using namespace engine;

static void PushObject(Object* obj);
static void LoopAll(std::vector<Object*>& Objects);
static void RenderAll(std::vector<Object*>& Objects, uint64_t zplane);
#ifndef ClassName
	#error please define a class name
#endif
#if defined(ATTRIBUTE_SOLID)
	#define LOCAL_CLASS_SOLID true
#else
	#define LOCAL_CLASS_SOLID false
#endif
#define LOCAL_CLASS_TRANSPARENCY 0.0f
#define LocalClassName ClassName
extern Shader __Shader_Generic_Object_Sqaure_Solid;
static LightingProperties_T LightingProperties(
	rgba(0,0,0,0),
	0.0,
	0.2,
	ENGINE_ZPLANE_NATURAL,
	rgba(0xFF,0xFF,0xFF,LOCAL_CLASS_TRANSPARENCY),
	0.0,
	0.0
);
static PhysicsProperties_T PhysicsProperties(
	1.0,
	1.0,
	0.0,
	0.0,
	1.0
);
static ObjectClass localClass(
	LocalClassName,
	nullptr,
	&__Shader_Generic_Object_Sqaure_Solid,
	&LightingProperties,
	&PhysicsProperties,
	false,
	nullptr,
	false,
	&RenderAll,
	&LoopAll,
	&PushObject
);

#include "Render_Generic_Atlas.h"

static void LoopAll(std::vector<Object*>& Objects){}

static void PushObject(Object* obj){}

__attribute__((constructor)) static void ctor()
{
	engine::PushObjectClass(&localClass);
}
