#include <Engine/platform.h>

//, ███████ ███    ██  ██████  ██ ███    ██ ███████
//, ██      ████   ██ ██       ██ ████   ██ ██
//, █████   ██ ██  ██ ██   ███ ██ ██ ██  ██ █████
//, ██      ██  ██ ██ ██    ██ ██ ██  ██ ██ ██
//, ███████ ██   ████  ██████  ██ ██   ████ ███████
#include <Engine/Window.hxx>
#include <Engine/Position.hxx>
#include <Engine/Size.hxx>
#include <Engine/Except.hxx>

namespace engine
{
	Position2D::Position2D(uint64_t x, uint64_t y)
		:x(x),y(y){}

	RenderPosition2D::RenderPosition2D(double x, double y)
		:x(x),y(y){}

	RenderPosition2D::RenderPosition2D(Position2D& position)
	{
		if(currentWindow == nullptr)
			throw BadCurrentWindowException(currentWindow,"[ENGINE] assertion failed: currentWindow != nullptr");
		if(currentWindow->Height == 0)
			throw BadCurrentWindowException(currentWindow,"[ENGINE] assertion failed: currentWindow->Height != 0");
		if(currentWindow->Width == 0)
			throw BadCurrentWindowException(currentWindow,"[ENGINE] assertion failed: currentWindow->Width != 0");
		double unitWidth = 0.000000002;
		if(currentWindow->Width > currentWindow->Height)
		{
			double ratio = (double)currentWindow->Width/(double)currentWindow->Height;
			this->x = (-1)+(position.x*unitWidth      	);
			this->y = (-1)+(position.y*unitWidth*ratio	);
		}
		else if(currentWindow->Height > currentWindow->Width)
		{
			double ratio = (double)currentWindow->Height/(double)currentWindow->Width;
			this->x = (-1)+(position.x*unitWidth*ratio	);
			this->y = (-1)+(position.y*unitWidth		);
		}
		else
		{
			this->x = (-1)+(position.x*unitWidth);
			this->y = (-1)+(position.y*unitWidth);
		}
	}
}
