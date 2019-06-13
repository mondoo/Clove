#include "clvpch.hpp"
#include "Mouse.hpp"

namespace clv{
	Mouse::Event::Event() = default;

	Mouse::Event::Event(Type type, MouseButton button, int x, int y)
		: type(type)
		, button(button)
		, x(x)
		, y(y){
	}

	Mouse::Mouse() = default;

	Mouse::~Mouse() = default;

	std::optional<Mouse::Event> Mouse::getEvent(){
		if(!isBufferEmpty()){
			Mouse::Event e = buffer.front();
			buffer.pop();
			return e;
		} else{
			return {};
		}
	}

	void Mouse::flush(){
		buffer = std::queue<Mouse::Event>();
	}

	void Mouse::onMouseMove(int x, int y){
		this->x = x;
		this->y = y;

		buffer.push({ Mouse::Event::Type::Move, MouseButton::None, x, y });
		trimBuffer();
	}

	void Mouse::onButtonPressed(MouseButton button, int x, int y){
		this->x = x;
		this->y = y;

		buttonStates[button] = true;
		buffer.push({ Mouse::Event::Type::Pressed, button, x, y });
		trimBuffer();
	}

	void Mouse::onButtonReleased(MouseButton button, int x, int y){
		this->x = x;
		this->y = y;

		buttonStates[button] = false;
		buffer.push({ Mouse::Event::Type::Released, button, x, y });
		trimBuffer();
	}

	void Mouse::onWheelDelta(int delta, int x, int y){
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

	void Mouse::onWheelUp(int x, int y){
		this->x = x;
		this->y = y;

		buffer.push({ Mouse::Event::Type::WheelUp, MouseButton::None, x, y });
		trimBuffer();
	}

	void Mouse::onWheelDown(int x, int y){
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
		buttonStates.empty();
	}

	void Mouse::trimBuffer(){
		while(buffer.size() > bufferSize){
			buffer.pop();
		}
	}
}