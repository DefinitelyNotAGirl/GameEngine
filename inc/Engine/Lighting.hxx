#pragma once

#include "platform.h"
//stdint & co.
#include "common.hxx"
//Engine
#include "Position.hxx"
#include "Size.hxx"
#include "RGBA.hxx"

namespace engine
{
	class LightingProperties_T {
	public:
		rgba Color;//set to Alpha to 0 for objects that dont emit light
		float Transparency;// value between 0 and 1, 0 being opaque and 1 being fully transparent
		float ReflectionStrength; // value between 0 and 1, 0 means no light gets reflected from this object, 1 means all light gets reflected from this object
		rgba ReflectionMod; // out_light = in_light * ReflectionMod
		float LightAngle; // angle at which light emitted by this object spreads into the scene, value between -90.0 and 90.0
		LightingProperties_T(
			rgba Color,
			float Transparency,
			float ReflectionStrength,
			rgba ReflectionMod,
			float LightAngle
		);
	};
}
