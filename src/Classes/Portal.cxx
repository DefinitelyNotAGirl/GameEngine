#define ClassName "Class.Level.Portal"
#define ATLAS_POS_X 13
#define ATLAS_POS_Y 22

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
#define LOCAL_CLASS_TRANSPARENCY 0.0f
#define LocalClassName ClassName
extern Shader __Shader_Generic_Object_Sqaure_Solid;
static LightingProperties_T LightingProperties(
	rgba(0.5f,0.0f,1.0f,1.0f),
	0.0,
	0.2,
	(1<<0)|(1<<1)|(1<<2),
	rgba(1.0f,0.2f,1.0f,LOCAL_CLASS_TRANSPARENCY),
	0.0,
	6.0f
);
static PhysicsProperties_T PhysicsProperties(
	1.0,
	0.7,
	0.0,
	0.0,
	0.8
);
static ObjectClass localClass(
	LocalClassName,
	nullptr,
	&__Shader_Generic_Object_Sqaure_Solid,
	&LightingProperties,
	&PhysicsProperties,
	false,
	nullptr,
	true,
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
