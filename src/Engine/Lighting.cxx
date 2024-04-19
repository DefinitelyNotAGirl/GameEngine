#include <Engine/Lighting.hxx>

namespace engine {
	LightingProperties_T::LightingProperties_T(
		rgba Color,
		float Transparency,
		float ReflectionStrength,
		rgba ReflectionMod,
		float LightAngle
	):
		Color(Color),
		Transparency(Transparency),
		ReflectionStrength(ReflectionStrength),
		ReflectionMod(ReflectionMod),
		LightAngle(LightAngle)
	{
	}
}