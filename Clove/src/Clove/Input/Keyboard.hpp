#pragma once

#include "Clove/Input/KeyCodes.hpp"

#include <bitset>
#include <queue>
#include <optional>

namespace clv{
	class Keyboard{
	#if CLV_PLATFORM_WINDOWS
		friend class WindowsWindow;
	#endif
		
		//DEFINITIONS
	public:
		struct Event{
			//DEFINITIONS
		public:
			enum class Type{
				Pressed,
				Released,
				Invalid
			};

			//VARIABLES
		private:
			Type type = Type::Invalid;
			Key key = Key::None;

			//FUNCTIONS
		public:
			Event();
			Event(Type type, Key key);

			inline bool isPressed() const;
			inline bool isReleased() const;
			inline bool isValid() const;

			inline Key getKey() const;
		};

		//VARIABLES
	private:
		static constexpr unsigned int numKeys		= 256u;
		static constexpr unsigned int bufferSize	= 16u;

		bool autoRepeatEnabled = false;

		std::bitset<numKeys> keyStates;
		std::queue<Event> keyBuffer;
		std::queue<char> charBuffer;

		//FUNCTIONS
	public:
		Keyboard();
		Keyboard(const Keyboard& other) = delete;
		Keyboard(Keyboard&& other) noexcept = delete;

		~Keyboard();

		inline bool isKeyPressed(Key key) const;
		std::optional<Event> getKeyEvent();
		inline bool isKeyBufferEmpty() const;

		std::optional<char> getCharEvent();
		inline bool isCharBufferEmpty() const;

		void flushKeyBuffer();
		void flushCharBuffer();
		void flush();

		inline void enableAutoRepeat();
		inline void disableAutoRepeat();
		inline bool isAutoRepeatEnabled() const;

		Keyboard& operator=(const Keyboard& other) = delete;
		Keyboard& operator=(Keyboard&& other) noexcept = delete;

	private:
		void onKeyPressed(Key key);
		void onKeyReleased(Key key);

		void onChar(char character);

		void clearState();

		template<typename T>
		static void trimBuffer(std::queue<T>& buffer);
	};
}

#include "Keyboard.inl"