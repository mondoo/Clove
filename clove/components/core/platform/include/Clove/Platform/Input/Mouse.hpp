#pragma once

#include "Clove/Platform/Input/MouseButtonCodes.hpp"

#include <bitset>
#include <optional>
#include <queue>
#include <unordered_map>
#include <Clove/Maths/Vector.hpp>

namespace garlic::clove {
    class Mouse {
#if CLOVE_PLATFORM_WINDOWS
        friend class WindowsWindow;
#elif CLOVE_PLATFORM_LINUX
        friend class LinuxWindow;
#elif CLOVE_PLATFORM_MACOS
        friend class MacWindow;
#endif

        //TYPES
    public:
        struct Event {
            //TYPES
        public:
            enum class Type {
                Move,
                Pressed,
                Released,
                WheelUp,
                WheelDown,
                Enter,
                Leave,
                Invalid,
            };

            //VARIABLES
        private:
            Type type{ Type::Invalid };
            MouseButton button{ MouseButton::None };

            int32_t x{ 0 };
            int32_t y{ 0 };

            //FUNCTIONS
        public:
            Event();
            Event(Type type, MouseButton button, int32_t x, int32_t y);

            Type getType() const;
            bool isValid() const;

            std::pair<int32_t, int32_t> getPos() const;

            MouseButton getButton() const;
        };

        //VARIABLES
    private:
        static constexpr uint32_t bufferSize{ 16u };

        int32_t x{ 0 };
        int32_t y{ 0 };

        int32_t wheelDelta{ 0 };

        bool inWindow = false;

        std::unordered_map<MouseButton, bool> buttonStates;
        std::queue<Event> buffer;

        //FUNCTIONS
    public:
        Mouse();

        Mouse(Mouse const &other)     = delete;
        Mouse(Mouse &&other) noexcept = delete;

        Mouse &operator=(Mouse const &other) = delete;
        Mouse &operator=(Mouse &&other) noexcept = delete;

        ~Mouse();

        bool isButtonPressed(MouseButton button) const;
        std::optional<Event> getEvent();

        vec2i getPosition() const;

        bool isInWindow() const;

        bool isBufferEmpty() const;

        void flush();

    private:
        void onMouseMove(int32_t x, int32_t y);

        void onButtonPressed(MouseButton button, int32_t x, int32_t y);
        void onButtonReleased(MouseButton button, int32_t x, int32_t y);

        void onWheelDelta(int32_t delta, int32_t x, int32_t y);
        void onWheelUp(int32_t x, int32_t y);
        void onWheelDown(int32_t x, int32_t y);

        void onMouseLeave();
        void onMouseEnter();

        void clearState();

        void trimBuffer();
    };
}
