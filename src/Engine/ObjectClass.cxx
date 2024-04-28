#include <Engine/ObjectClass.hxx>
#include <cstring>
#include <iostream>
namespace engine
{
	ObjectClass::ObjectClass(
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
	):
		Name(Name),
		Texture(Texture),
		RenderAll(renderAll),
		LoopAll(loopAll),
		shader(shader),
		computeLighting(computeLighting),
		ColissionCallback(ColissionCallback),
		computePhysics(computePhysics),
		LightingProperties(LightingProperties),
		PhysicsProperties(PhysicsProperties),
		Push(push)
	{
	}

	std::vector<ObjectClass*> ObjectClasses;

	void ObjectClass::PushGeneric(Object* obj)
	{
		this->Objects[obj->zplane].push_back(obj);
	}

	ObjectClass* getObjectClass(char* classname)
	{
		for(ObjectClass* c : ObjectClasses)
		{
			if(strcmp(classname,c->Name) == 0)
			{
				return c;
			}
		}
		throw (ClassNotFoundException)classname;
	}

	void PushObjectClass(ObjectClass* c)
	{
		ObjectClasses.push_back(c);
	}

	void PushObjectGlobal(Object* obj)
	{
		if(obj->LightingProperties->zplanes == ENGINE_ZPLANE_NATURAL)
		{
			obj->LightingProperties->zplanes = 0;

			for(uint64_t i = 0;i<obj->zplane;i++)
				obj->LightingProperties->zplanes |= (1<<i);
		}
		obj->oclass->PushGeneric(obj);
		obj->oclass->Push(obj);
	}

	uint64_t zplaneCount;
	void setZPlaneCount(uint64_t count)
	{
		zplaneCount = count;
		for(ObjectClass* c : ObjectClasses)
			c->Objects.resize(count);
	}
}