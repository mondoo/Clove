#pragma once

#include "Clove/Input/MouseButtonCodes.hpp"

#include <bitset>
#include <queue>
#include <optional>

namespace clv{
	class Mouse{
	#if CLV_PLATFORM_WINDOWS
		friend class WindowsWindow;
	#elif CLV_PLATFORM_LINUX
		friend class LinuxWindow;
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

			Type getType() const;
			bool isValid() const;

			std::pair<int, int> getPos() const;

			MouseButton getButton() const;
		};

		//VARIABLES
	private:
		static constexpr unsigned int bufferSize = 16u;

		int x = 0;
		int y = 0;

		int wheelDelta = 0;

		bool inWindow = false;

		std::unordered_map<MouseButton, bool> buttonStates;
		std::queue<Event> buffer;

		//FUNCTIONS
	public:
		Mouse();
		Mouse(const Mouse& other) = delete;
		Mouse(Mouse&& other) noexcept = delete;

		~Mouse();

		bool isButtonPressed(MouseButton button) const;
		std::optional<Event> getEvent();
		
		std::pair<int, int> getPosition() const;

		bool isInWindow() const;
		
		bool isBufferEmpty() const;

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