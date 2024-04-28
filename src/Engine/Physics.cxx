#include <Engine/Physics.hxx>

namespace engine
{
	PhysicsProperties_T::PhysicsProperties_T(
		double Gravity,
		double Resistance,
		double Velocity,
		double Direction,
		double Friction
	):
		Gravity(Gravity),
		Resistance(Resistance),
		Velocity(Velocity),
		Direction(Direction),
		Friction(Friction)
	{
	}
}