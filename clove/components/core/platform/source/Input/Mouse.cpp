#include "Clove/Platform/Input/Mouse.hpp"

namespace garlic::clove {
    Mouse::Event::Event() = default;

    Mouse::Event::Event(Type type, MouseButton button, vec2i pos)
        : type{ type }
        , button{ button }
        , pos{ std::move(pos) } {
    }

    Mouse::Event::Type Mouse::Event::getType() const {
        return type;
    }

    bool Mouse::Event::isValid() const {
        return type != Type::Invalid && button > MouseButton::Undefined;
    }

    vec2i const &Mouse::Event::getPos() const {
        return pos;
    }

    MouseButton Mouse::Event::getButton() const {
        return button;
    }

    Mouse::Dispatcher::Dispatcher() = default;

    Mouse::Dispatcher::Dispatcher(Dispatcher const &other) = default;

    Mouse::Dispatcher::Dispatcher(Dispatcher &&other) noexcept = default;

    Mouse::Dispatcher &Mouse::Dispatcher::operator=(Dispatcher const &other) = default;

    Mouse::Dispatcher &Mouse::Dispatcher::operator=(Dispatcher &&other) noexcept = default;

    Mouse::Dispatcher::~Dispatcher() = default;

    void Mouse::Dispatcher::onMouseMove(int32_t x, int32_t y) {
        pos = { x, y };

        buffer.push({ Mouse::Event::Type::Move, MouseButton::None, pos });
        trimBuffer();
    }

    void Mouse::Dispatcher::onButtonPressed(MouseButton button, int32_t x, int32_t y) {
        pos = { x, y };

        buttonStates[button] = true;
        buffer.push({ Mouse::Event::Type::Pressed, button, pos });
        trimBuffer();
    }

    void Mouse::Dispatcher::onButtonReleased(MouseButton button, int32_t x, int32_t y) {
        pos = { x, y };

        buttonStates[button] = false;
        buffer.push({ Mouse::Event::Type::Released, button, pos });
        trimBuffer();
    }

    void Mouse::Dispatcher::onWheelDelta(int32_t delta, int32_t x, int32_t y) {
        wheelDelta += delta;
        while(wheelDelta >= CLV_WHEEL_DELTA) {
            wheelDelta -= CLV_WHEEL_DELTA;
            onWheelUp(x, y);
        }
        while(wheelDelta <= -CLV_WHEEL_DELTA) {
            wheelDelta += CLV_WHEEL_DELTA;
            onWheelDown(x, y);
        }
    }

    void Mouse::Dispatcher::onWheelUp(int32_t x, int32_t y) {
        pos = { x, y };

        buffer.push({ Mouse::Event::Type::WheelUp, MouseButton::None, pos });
        trimBuffer();
    }

    void Mouse::Dispatcher::onWheelDown(int32_t x, int32_t y) {
        pos = { x, y };

        buffer.push({ Mouse::Event::Type::WheelDown, MouseButton::None, pos });
        trimBuffer();
    }

    void Mouse::Dispatcher::onMouseLeave() {
        inWindow = false;

        buffer.push({ Mouse::Event::Type::Leave, MouseButton::None, pos });
        trimBuffer();
    }

    void Mouse::Dispatcher::onMouseEnter() {
        inWindow = true;

        buffer.push({ Mouse::Event::Type::Enter, MouseButton::None, pos });
        trimBuffer();
    }

    void Mouse::Dispatcher::clearState() {
        buttonStates.clear();
    }

    void Mouse::Dispatcher::trimBuffer() {
        while(buffer.size() > bufferSize) {
            buffer.pop();
        }
    }

    Mouse::Mouse(Dispatcher &dispatcher)
        : dispatcher{ dispatcher } {
    }

    Mouse::~Mouse() = default;

    bool Mouse::isButtonPressed(MouseButton button) const {
        if(const auto buttonIt = dispatcher.buttonStates.find(button); buttonIt != dispatcher.buttonStates.end()) {
            return buttonIt->second;
        } else {
            return false;
        }
    }

    std::optional<Mouse::Event> Mouse::getEvent() {
        if(!isBufferEmpty()) {
            Mouse::Event e{ std::move(dispatcher.buffer.front()) };
            dispatcher.buffer.pop();
            return e;
        } else {
            return {};
        }
    }

    vec2i const &Mouse::getPosition() const {
        return dispatcher.pos;
    }

    bool Mouse::isInWindow() const {
        return dispatcher.inWindow;
    }

    bool Mouse::isBufferEmpty() const {
        return dispatcher.buffer.empty();
    }

    void Mouse::flush() {
        dispatcher.buffer = std::queue<Mouse::Event>();
    }
}
