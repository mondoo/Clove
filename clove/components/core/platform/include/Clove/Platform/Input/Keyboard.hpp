#pragma once

#include "Clove/Platform/Input/KeyCodes.hpp"

#include <bitset>
#include <optional>
#include <queue>
#include <unordered_map>

namespace garlic::clove {
    class Keyboard {
#if GARLIC_PLATFORM_WINDOWS
        friend class WindowsWindow;
#elif GARLIC_PLATFORM_LINUX
        friend class LinuxWindow;
#elif GARLIC_PLATFORM_MACOS
        friend class MacWindow;
#endif

        //TYPES
    public:
        struct Event {
            //TYPES
        public:
            enum class Type {
                Pressed,
                Released,
                Invalid
            };

            //VARIABLES
        private:
            Type type{ Type::Invalid };
            Key key{ Key::None };

            //FUNCTIONS
        public:
            Event();
            Event(Type type, Key key);

            bool isPressed() const;
            bool isReleased() const;
            bool isValid() const;

            Type getType() const;
            Key getKey() const;
        };

        //VARIABLES
    private:
        static constexpr uint32_t bufferSize{ 16u };

        bool autoRepeatEnabled{ false };

        std::unordered_map<Key, bool> keyStates;
        std::queue<Event> keyBuffer;
        std::queue<char> charBuffer;

        //FUNCTIONS
    public:
        Keyboard();

        Keyboard(Keyboard const &other)     = delete;
        Keyboard(Keyboard &&other) noexcept = delete;

        Keyboard &operator=(Keyboard const &other) = delete;
        Keyboard &operator=(Keyboard &&other) noexcept = delete;

        ~Keyboard();

        bool isKeyPressed(Key key) const;
        std::optional<Event> getKeyEvent();
        bool isKeyBufferEmpty() const;

        std::optional<char> getCharEvent();
        bool isCharBufferEmpty() const;

        void flushKeyBuffer();
        void flushCharBuffer();
        void flush();

        void enableAutoRepeat();
        void disableAutoRepeat();
        bool isAutoRepeatEnabled() const;

    private:
        void onKeyPressed(Key key);
        void onKeyReleased(Key key);

        void onChar(char character);

        void clearState();

        template<typename T>
        static void trimBuffer(std::queue<T> &buffer);
    };
}

#include "Keyboard.inl"
