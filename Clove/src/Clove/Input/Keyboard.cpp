#include "clvpch.hpp"
#include "Keyboard.hpp"

namespace clv{
	Keyboard::Event::Event() = default;

	Keyboard::Event::Event(Type type, Key key)
		: type(type)
		, key(key){
	}

	Keyboard::Keyboard() = default;

	Keyboard::~Keyboard() = default;

	std::optional<Keyboard::Event> Keyboard::getKeyEvent(){
		if(!isKeyBufferEmpty()){
			Keyboard::Event e = keyBuffer.front();
			keyBuffer.pop();
			return e;
		} else{
			return {};
		}
	}

	std::optional<char> Keyboard::getCharEvent(){
		if(!isCharBufferEmpty()){
			char charCode = charBuffer.front();
			charBuffer.pop();
			return charCode;
		} else{
			return {};
		}
	}

	void clv::Keyboard::flushKeyBuffer(){
		keyBuffer = std::queue<Event>();
	}

	void Keyboard::flushCharBuffer(){
		charBuffer = std::queue<char>();
	}

	void Keyboard::flush(){
		flushKeyBuffer();
		flushCharBuffer();
	}

	void Keyboard::onKeyPressed(Key key){
		keyStates[static_cast<char>(key)] = true;
		keyBuffer.push({ Keyboard::Event::Type::Pressed, key });
		trimBuffer(keyBuffer);
	}

	void Keyboard::onKeyReleased(Key key){
		keyStates[static_cast<char>(key)] = false;
		keyBuffer.push({ Keyboard::Event::Type::Released, key });
		trimBuffer(keyBuffer);
	}

	void Keyboard::onChar(char character){
		charBuffer.push(character);
		trimBuffer(charBuffer);
	}

	void Keyboard::clearState(){
		keyStates.reset();
	}
}