#pragma once
#include "common.hxx"

namespace engine
{
	class rgba {
	public:
		float Red;
		float Green;
		float Blue;
		float Alpha;
		rgba(uint8_t Red,uint8_t Green,uint8_t Blue, uint8_t Alpha);
	};
}
