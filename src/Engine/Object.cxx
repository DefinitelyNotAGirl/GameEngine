#include <Engine/Object.hxx>
#include <Engine/ObjectClass.hxx>

namespace engine
{
	Object::Object(
		Position2D Pos,
		Size2D Size,
		Size2D Hitbox,
		uint64_t zplane,
		void* classData,
		ObjectClass* oclass
	):
	Pos(Pos),
	Size(Size),
	Hitbox(Hitbox),
	zplane(zplane),
	classData(classData),
	oclass(oclass)
	{
		this->LightingProperties = this->oclass->LightingProperties;
		this->PhysicsProperties = this->oclass->PhysicsProperties;
	}
}