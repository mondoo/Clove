#pragma once

#include "Clove/Platform/Input/MouseButtonCodes.hpp"

#include <Clove/Maths/Vector.hpp>
#include <bitset>
#include <optional>
#include <queue>
#include <unordered_map>

namespace garlic::clove {
    class Mouse {
        //TYPES
    public:
        /**
         * @brief Represents a single mouse event (clicked, moved etc.)
         */
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

            vec2i pos{};

            //FUNCTIONS
        public:
            Event();
            Event(Type type, MouseButton button, vec2i pos);

            Type getType() const;
            bool isValid() const;

            vec2i const &getPos() const;

            MouseButton getButton() const;
        };

        /**
         * @brief Recieves events for a Mouse object 
         */
        class Dispatcher {
            friend Mouse;

            //VARIABLES
        private:
            static constexpr uint32_t bufferSize{ 16u };

            vec2i pos{};

            int32_t wheelDelta{ 0 };

            bool inWindow = false;

            std::unordered_map<MouseButton, bool> buttonStates;
            std::queue<Event> buffer;

            //FUNCTIONS
        public:
            Dispatcher();

            Dispatcher(Dispatcher const &other);
            Dispatcher(Dispatcher&& other) noexcept;
            
            Dispatcher &operator=(Dispatcher const &other);
            Dispatcher &operator=(Dispatcher &&other) noexcept;

            ~Dispatcher();

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

        //VARIABLES
    private:
        Dispatcher &dispatcher;

        //FUNCTIONS
    public:
        Mouse() = delete;
        Mouse(Dispatcher &dispatcher);

        Mouse(Mouse const &other)     = delete;
        Mouse(Mouse &&other) noexcept = delete;

        Mouse &operator=(Mouse const &other) = delete;
        Mouse &operator=(Mouse &&other) noexcept = delete;

        ~Mouse() = default;

        bool isButtonPressed(MouseButton button) const;
        std::optional<Event> getEvent();

        vec2i const &getPosition() const;

        bool isInWindow() const;

        bool isBufferEmpty() const;

        void flush();
    };
}
