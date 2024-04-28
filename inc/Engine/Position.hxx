#pragma once
#include "common.hxx"

namespace engine
{
	class Position2D {
	public:
		uint64_t x;
		uint64_t y;
		Position2D(uint64_t x,uint64_t y);
	};

	class RenderPosition2D {
	public:
		double x;
		double y;
		RenderPosition2D();
		RenderPosition2D(double x,double y);
		/**
		 * @throws Throws a BadCurrentWindowException if currentWindow is nullptr or if either currentWindow->Width or currentWindow->Height are either 0 or >1.000.000.000
		*/
		RenderPosition2D(Position2D& position);
	};
}
