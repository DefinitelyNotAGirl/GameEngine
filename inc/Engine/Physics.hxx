#pragma once
#include <Engine/common.hxx>
namespace engine
{
	class PhysicsProperties_T {
	public:
		double Gravity;
		double Resistance;
		double Velocity;
		double Direction;// 0 ==> moving to the right (x+=...)
		double Friction;
		PhysicsProperties_T(
			double Gravity,
			double Resistance,
			double Velocity,
			double Direction,
			double Friction
		);
	};
}
