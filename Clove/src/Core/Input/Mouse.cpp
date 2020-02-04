#include "Clove/Core/Input/Mouse.hpp"

namespace clv{
	Mouse::Event::Event() = default;

	Mouse::Event::Event(Type type, MouseButton button, int32_t x, int32_t y)
		: type(type)
		, button(button)
		, x(x)
		, y(y){
	}

	Mouse::Event::Type Mouse::Event::getType() const{
		return type;
	}

	bool Mouse::Event::isValid() const{
		return type != Type::Invalid && button > MouseButton::Undefined;
	}

	std::pair<int32_t, int32_t> Mouse::Event::getPos() const{
		return { x, y };
	}

	MouseButton Mouse::Event::getButton() const{
		return button;
	}

	Mouse::Mouse() = default;

	Mouse::~Mouse() = default;

	bool Mouse::isButtonPressed(MouseButton button) const{
		if(const auto buttonIt = buttonStates.find(button); buttonIt != buttonStates.end()){
			return buttonIt->second;
		} else{
			return false;
		}
	}

	std::optional<Mouse::Event> Mouse::getEvent(){
		if(!isBufferEmpty()){
			Mouse::Event e = buffer.front();
			buffer.pop();
			return e;
		} else{
			return {};
		}
	}

	std::pair<int, int> Mouse::getPosition() const{
		return { x, y };
	}

	bool Mouse::isInWindow() const{
		return inWindow;
	}

	bool Mouse::isBufferEmpty() const{
		return buffer.empty();
	}

	void Mouse::flush(){
		buffer = std::queue<Mouse::Event>();
	}

	void Mouse::onMouseMove(int32_t x, int32_t y){
		this->x = x;
		this->y = y;

		buffer.push({ Mouse::Event::Type::Move, MouseButton::None, x, y });
		trimBuffer();
	}

	void Mouse::onButtonPressed(MouseButton button, int32_t x, int32_t y){
		this->x = x;
		this->y = y;

		buttonStates[button] = true;
		buffer.push({ Mouse::Event::Type::Pressed, button, x, y });
		trimBuffer();
	}

	void Mouse::onButtonReleased(MouseButton button, int32_t x, int32_t y){
		this->x = x;
		this->y = y;

		buttonStates[button] = false;
		buffer.push({ Mouse::Event::Type::Released, button, x, y });
		trimBuffer();
	}

	void Mouse::onWheelDelta(int32_t delta, int32_t x, int32_t y){
		wheelDelta += delta;
		while(wheelDelta >= CLV_WHEEL_DELTA){
			wheelDelta -= CLV_WHEEL_DELTA;
			onWheelUp(x, y);
		}
		while(wheelDelta <= -CLV_WHEEL_DELTA){
			wheelDelta += CLV_WHEEL_DELTA;
			onWheelDown(x, y);
		}
	}

	void Mouse::onWheelUp(int32_t x, int32_t y){
		this->x = x;
		this->y = y;

		buffer.push({ Mouse::Event::Type::WheelUp, MouseButton::None, x, y });
		trimBuffer();
	}

	void Mouse::onWheelDown(int32_t x, int32_t y){
		this->x = x;
		this->y = y;

		buffer.push({ Mouse::Event::Type::WheelDown, MouseButton::None, x, y });
		trimBuffer();
	}

	void Mouse::onMouseLeave(){
		inWindow = false;

		buffer.push({ Mouse::Event::Type::Leave, MouseButton::None, x, y });
		trimBuffer();
	}

	void Mouse::onMouseEnter(){
		inWindow = true;

		buffer.push({ Mouse::Event::Type::Enter, MouseButton::None, x, y });
		trimBuffer();
	}

	void Mouse::clearState(){
		buttonStates.clear();
	}

	void Mouse::trimBuffer(){
		while(buffer.size() > bufferSize){
			buffer.pop();
		}
	}
}
