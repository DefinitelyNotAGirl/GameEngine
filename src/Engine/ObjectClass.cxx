#include <Engine/ObjectClass.hxx>
#include <cstring>
namespace engine
{
	ObjectClass::ObjectClass(
		char* Name,
		char* Texture,
		Shader* shader,
		LightingProperties_T* LightingProperties,
		ObjectClassIterator renderAll,
		ObjectClassIterator loopAll,
		PushObjectFunction push
	):
		Name(Name),
		Texture(Texture),
		RenderAll(renderAll),
		LoopAll(loopAll),
		shader(shader),
		LightingProperties(LightingProperties),
		Push(push)
	{
	}

	void ObjectClass::PushGeneric(Object* obj)
	{
		this->Objects.push_back(obj);
	}

	std::vector<ObjectClass*> ObjectClasses;

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

	void PushObject(ObjectClass* c,Object* obj)
	{
		c->PushGeneric(obj);
		c->Push(obj);
	}
}