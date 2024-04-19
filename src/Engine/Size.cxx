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
	Size2D::Size2D(uint64_t width, uint64_t height)
		:width(width),height(height){}

	RenderSize2D::RenderSize2D(Size2D& size)
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
			this->width  = (0)+(size.width*unitWidth      	);
			this->height = (0)+(size.height*unitWidth*ratio);
		}
		else if(currentWindow->Height > currentWindow->Width)
		{
			double ratio = (double)currentWindow->Height/(double)currentWindow->Width;
			this->width  = (0)+(size.width*unitWidth*ratio	);
			this->height = (0)+(size.height*unitWidth		);
		}
		else
		{
			this->width  = (0)+(size.width *unitWidth);
			this->height = (0)+(size.height*unitWidth);
		}
	}
}
