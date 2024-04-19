#include <Engine/Object.hxx>

namespace engine
{
	Object::Object(
		Position2D Pos,
		Size2D Size,
		Size2D Hitbox,
		bool hasGravity,
		bool hasColision,
		void* classData,
		ObjectClass* oclass
	):
	Pos(Pos),
	Size(Size),
	Hitbox(Hitbox),
	hasGravity(hasGravity),
	hasColision(hasColision),
	classData(classData),
	oclass(oclass)
	{
	}
}