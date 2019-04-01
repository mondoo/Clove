#include "clvpch.h"
#include "KeyEvent.h"

namespace clv{
	inline Key KeyEvent::getKey() const{
		return key;
	}

	KeyEvent::KeyEvent(Key inKey) 
		: key(inKey){
	}

	KeyPressedEvent::KeyPressedEvent(Key inKey, int inRepeatCount)
		: KeyEvent(inKey)
		, repeatCount(inRepeatCount){
	}

	inline int KeyPressedEvent::getRepeateCount() const{
		return repeatCount;
	}

	std::string KeyPressedEvent::toString() const{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << key << " (" << repeatCount << " repeats)";
		return ss.str();
	}

	KeyTypedEvent::KeyTypedEvent(Key inKey)
		: KeyEvent(inKey){
	}

	std::string KeyTypedEvent::toString() const{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << key;
		return ss.str();
	}

	KeyReleasedEvent::KeyReleasedEvent(Key inKey)
		: KeyEvent(inKey){
	}

	std::string KeyReleasedEvent::toString() const{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << key;
		return ss.str();
	}
}