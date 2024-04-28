#pragma once

#include "platform.h"
//stdint & co.
#include "common.hxx"
//Engine
#include "Position.hxx"
#include "Size.hxx"
#include "RGBA.hxx"

#define ENGINE_ZPLANE_ALL     0xFFFFFFFFFFFFFFFF // all zplanes
#define ENGINE_ZPLANE_NATURAL 0xFFFFFFFFFFFFFFFE // all zplanes <= the light sources zplane

namespace engine
{
	class LightingProperties_T {
	public:
		rgba Color;//set to Alpha to 0 for objects that dont emit light
		float Transparency;// value between 0 and 1, 0 being opaque and 1 being fully transparent
		float ReflectionStrength; // value between 0 and 1, 0 means no light gets reflected from this object, 1 means all light gets reflected from this object
		rgba ReflectionMod; // out_light = in_light * ReflectionMod
		float LightAngle; // angle at which light emitted by this object spreads into the scene, value between -90.0 and 90.0
		float Reach; // how far the light reaches (bigger number ==> further)
		uint64_t zplanes;//if bit n is set on this variable the light source will act on z-plane n
		LightingProperties_T(
			rgba Color,
			float Transparency,
			float ReflectionStrength,
			uint64_t zplanes,
			rgba ReflectionMod,
			float LightAngle,
			float Reach
		);
	};
}
