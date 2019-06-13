namespace clv{
	inline Mouse::Event::Type Mouse::Event::getType() const{
		return type;
	}

	inline bool Mouse::Event::isValid() const{
		return type != Type::Invalid;
	}

	inline std::pair<int, int> Mouse::Event::getPos() const{
		return { x, y };
	}

	inline MouseButton Mouse::Event::getButton() const{
		return button;
	}

	inline bool Mouse::isButtonPressed(MouseButton button) const{
		if(const auto buttonIt = buttonStates.find(button); buttonIt != buttonStates.end()){
			return buttonIt->second;
		}else{
			return false;
		}
	}

	inline std::pair<int, int> Mouse::getPosition() const{
		return { x, y };
	}

	inline bool Mouse::isInWindow() const{
		return inWindow;
	}

	inline bool Mouse::isBufferEmpty() const{
		return buffer.empty();
	}
}