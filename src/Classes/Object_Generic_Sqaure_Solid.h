#define ENGINE_BACKEND
#include <Engine.hxx>
#include <iostream>
using namespace engine;

static void PushObject(Object* obj);
static void LoopAll(std::vector<Object*>& Objects);
static void RenderAll(std::vector<Object*>& Objects);
#ifndef ClassName
	#error please define a class name
#endif
#define LocalClassName ClassName
extern Shader __Shader_Generic_Object_Sqaure_Solid;
static LightingProperties_T LightingProperties(
	rgba(0,0,0,0),
	0.0,
	0.2,
	rgba(0xFF,0xFF,0xFF,0xFF),
	0.0
);
static ObjectClass localClass(
	LocalClassName,
	nullptr,
	&__Shader_Generic_Object_Sqaure_Solid,
	&LightingProperties,
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
