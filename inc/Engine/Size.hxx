#pragma once
#include "common.hxx"

namespace engine
{
	class Size2D {
	public:
		uint64_t width;
		uint64_t height;
		Size2D(uint64_t width,uint64_t height);
	};

	class RenderSize2D {
	public:
		double width;
		double height;
		RenderSize2D(Size2D& size);
	};
}
