namespace clv{
	inline bool Keyboard::Event::isPressed() const{
		return type == Type::Pressed;
	}

	inline bool Keyboard::Event::isReleased() const{
		return type == Type::Released;
	}

	inline bool Keyboard::Event::isValid() const{
		return type != Type::Invalid;
	}

	inline Key Keyboard::Event::getKey() const{
		return key;
	}

	inline bool Keyboard::isKeyPressed(Key key) const{
		if(const auto keyIt = keyStates.find(key); keyIt != keyStates.end()){
			return keyIt->second;
		}else{
			return false;
		}
	}

	inline bool Keyboard::isKeyBufferEmpty() const{
		return keyBuffer.empty();
	}

	inline bool Keyboard::isCharBufferEmpty() const{
		return charBuffer.empty();
	}

	inline void Keyboard::enableAutoRepeat(){
		autoRepeatEnabled = true;
	}

	inline void Keyboard::disableAutoRepeat(){
		autoRepeatEnabled = false;
	}

	inline bool Keyboard::isAutoRepeatEnabled() const{
		return autoRepeatEnabled;
	}

	template<typename T>
	inline void Keyboard::trimBuffer(std::queue<T>& buffer){
		while(buffer.size() > bufferSize){
			buffer.pop();
		}
	}
}