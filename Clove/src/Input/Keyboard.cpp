#include "Clove/Input/Keyboard.hpp"

namespace clv{
	Keyboard::Event::Event() = default;

	Keyboard::Event::Event(Type type, Key key)
		: type(type)
		, key(key){
	}

	bool Keyboard::Event::isPressed() const{
		return type == Type::Pressed;
	}

	bool Keyboard::Event::isReleased() const{
		return type == Type::Released;
	}

	bool Keyboard::Event::isValid() const{
		return type != Type::Invalid && key > Key::Undefined;
	}

	Keyboard::Event::Type Keyboard::Event::getType() const {
		return type;
	}

	Key Keyboard::Event::getKey() const{
		return key;
	}

	Keyboard::Keyboard() = default;

	Keyboard::~Keyboard() = default;

	bool Keyboard::isKeyPressed(Key key) const{
		if(const auto keyIt = keyStates.find(key); keyIt != keyStates.end()){
			return keyIt->second;
		} else{
			return false;
		}
	}

	bool Keyboard::isKeyBufferEmpty() const{
		return keyBuffer.empty();
	}

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

	bool Keyboard::isCharBufferEmpty() const{
		return charBuffer.empty();
	}

	void Keyboard::flush(){
		flushKeyBuffer();
		flushCharBuffer();
	}

	void Keyboard::enableAutoRepeat(){
		autoRepeatEnabled = true;
	}

	void Keyboard::disableAutoRepeat(){
		autoRepeatEnabled = false;
	}

	bool Keyboard::isAutoRepeatEnabled() const{
		return autoRepeatEnabled;
	}

	void Keyboard::onKeyPressed(Key key){
		keyStates[key] = true;
		keyBuffer.push({ Keyboard::Event::Type::Pressed, key });
		trimBuffer(keyBuffer);
	}

	void Keyboard::onKeyReleased(Key key){
		keyStates[key] = false;
		keyBuffer.push({ Keyboard::Event::Type::Released, key });
		trimBuffer(keyBuffer);
	}

	void Keyboard::onChar(char character){
		charBuffer.push(character);
		trimBuffer(charBuffer);
	}

	void Keyboard::clearState(){
		keyStates.clear();
	}
}
