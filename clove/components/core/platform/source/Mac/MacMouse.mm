#include "Clove/Platform/Input/Mouse.hpp"
#include "Clove/Platform/Mac/CloveMac.hpp"

namespace garlic::clove {
	vec2i Mouse::getPosition() const {
		return {};
	}

	void Mouse::setPosition(vec2i const &position) {
		CGWarpMouseCursorPosition(CGPointMake(position.x, position.y));
		
		//CGWarpMouseCursorPosition doesn't generate a move event so spoof our own
		dispatcher.onMouseMove(position);
	}

	void Mouse::show(bool shouldShow) {
		if(shouldShow) {
			[NSCursor hide];
		} else {
			[NSCursor unhide];
		}
	}
}
