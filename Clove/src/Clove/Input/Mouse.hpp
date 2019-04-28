#pragma once

#include "Clove/Input/MouseButtonCodes.hpp"

#include <bitset>
#include <queue>
#include <optional>

namespace clv{
	class Mouse{
	#if CLV_PLATFORM_WINDOWS
		friend class WindowsWindow;
	#endif

		//DEFINITIONS
	public:
		struct Event{
			//DEFINITIONS
		public:
			enum class Type{
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
			Type type = Type::Invalid;
			MouseButton button = MouseButton::None;
			
			int x = 0;
			int y = 0;

			//FUNCTIONS
		public:
			Event();
			Event(Type type, MouseButton button, int x, int y);

			inline Type getType() const;
			inline bool isValid() const;

			inline std::pair<int, int> getPos() const;

			inline MouseButton getButton() const;
		};

		//VARIABLES
	private:
		static constexpr unsigned int numButtons = 256u;
		static constexpr unsigned int bufferSize = 16u;

		int x = 0;
		int y = 0;

		int wheelDelta = 0;

		bool inWindow = false;

		std::bitset<numButtons> buttonStates;
		std::queue<Event> buffer;

		//FUNCTIONS
	public:
		Mouse();
		Mouse(const Mouse& other) = delete;
		Mouse(Mouse&& other) noexcept = delete;

		~Mouse();

		inline bool isButtonPressed(MouseButton button) const;
		std::optional<Event> getEvent();
		
		inline std::pair<int, int> getPosition() const;

		inline bool isInWindow() const;
		
		inline bool isBufferEmpty() const;

		void flush();

		Mouse& operator=(const Mouse& other) = delete;
		Mouse& operator=(Mouse&& other) noexcept = delete;

	private:
		void onMouseMove(int x, int y);

		void onButtonPressed(MouseButton button, int x, int y);
		void onButtonReleased(MouseButton button, int x, int y);

		void onWheelDelta(int delta, int x, int y);
		void onWheelUp(int x, int y);
		void onWheelDown(int x, int y);

		void onMouseLeave();
		void onMouseEnter();

		void clearState();

		void trimBuffer();
	};
}

#include "Mouse.inl"