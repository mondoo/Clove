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
	#elif CLV_PLATFORM_MACOS
		friend class MacWindow;
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
			
			int32 x = 0;
			int32 y = 0;

			//FUNCTIONS
		public:
			Event();
			Event(Type type, MouseButton button, int32 x, int32 y);

			Type getType() const;
			bool isValid() const;

			std::pair<int32, int32> getPos() const;

			MouseButton getButton() const;
		};

		//VARIABLES
	private:
		static constexpr uint32 bufferSize = 16u;

		int32 x = 0;
		int32 y = 0;

		int32 wheelDelta = 0;

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
		
		std::pair<int32, int32> getPosition() const;

		bool isInWindow() const;
		
		bool isBufferEmpty() const;

		void flush();

		Mouse& operator=(const Mouse& other) = delete;
		Mouse& operator=(Mouse&& other) noexcept = delete;

	private:
		void onMouseMove(int32 x, int32 y);

		void onButtonPressed(MouseButton button, int32 x, int32 y);
		void onButtonReleased(MouseButton button, int32 x, int32 y);

		void onWheelDelta(int32 delta, int32 x, int32 y);
		void onWheelUp(int32 x, int32 y);
		void onWheelDown(int32 x, int32 y);

		void onMouseLeave();
		void onMouseEnter();

		void clearState();

		void trimBuffer();
	};
}
