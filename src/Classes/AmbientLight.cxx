#define ClassName "Class.Level.AmbientLight"

#define ENGINE_BACKEND
#include <Engine.hxx>
#include <iostream>
using namespace engine;

static void PushObject(Object* obj);
static void LoopAll(std::vector<Object*>& Objects);
static void RenderAll(std::vector<Object*>& Objects,uint64_t zplane);
#ifndef ClassName
	#error please define a class name
#endif
#define LOCAL_CLASS_TRANSPARENCY 0.0f
#define LocalClassName ClassName
extern Shader __Shader_Generic_Object_Sqaure_Solid;
static PhysicsProperties_T PhysicsProperties(0.0,0.0,0.0,0.0,0.0);
static LightingProperties_T LightingProperties(
	rgba(0.0f,0.25f,1.0f,0.100f), //. night
	//rgba(1.0f,1.0f,1.0f,1.0f), //. all white
	0.0,
	0.2,
	ENGINE_ZPLANE_ALL,
	rgba(1.0f,0.2f,1.0f,LOCAL_CLASS_TRANSPARENCY),
	0.0,
	0.0f
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

static void RenderAll(std::vector<Object*>& Objects,uint64_t zplane){}
static void LoopAll(std::vector<Object*>& Objects){}
static void PushObject(Object* obj){}
__attribute__((constructor)) static void ctor()
{
	engine::PushObjectClass(&localClass);
}
