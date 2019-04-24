namespace clv{
	inline float MouseMovedEvent::getX() const{
		return mouseX;
	}

	inline float MouseMovedEvent::getY() const{
		return mouseY;
	}

	inline float MouseScrolledEvent::getXOffset() const{
		return xOffset;
	}

	inline float MouseScrolledEvent::getYOffset() const{
		return yOffset;
	}
}