#include "clvpch.h"
#include "ApplicationEvent.h"

namespace clv{
	WindowResizeEvent::WindowResizeEvent(unsigned int inWidth, unsigned int inHeight)
		: width(inWidth)
		, height(inHeight){
	}

	inline unsigned int WindowResizeEvent::getWidth() const{
		return width;
	}

	inline unsigned int WindowResizeEvent::getHeight() const{
		return height;
	}

	std::string WindowResizeEvent::toString() const{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << width << " , " << height;
		return ss.str();
	}
}