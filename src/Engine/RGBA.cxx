#include <Engine/RGBA.hxx>

namespace engine
{
	rgba::rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		this->Red = r == 0 ? 0 : (float)255/r;
		this->Green = g == 0 ? 0 : (float)255/g;
		this->Blue = b == 0 ? 0 : (float)255/b;
		this->Alpha = a == 0 ? 0 : (float)255/a;
	}
}
