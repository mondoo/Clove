#pragma once

namespace clv{
	enum class MouseButton{
		None	= -1,

	#if CLV_PLATFORM_WINDOWS
		_1		= MK_LBUTTON,
		_2		= MK_RBUTTON,
		_3		= MK_MBUTTON,
		_4		= MK_XBUTTON1,
		_5		= MK_XBUTTON2,
	#elif CLV_PLATFORM_LINUX
		_1		= Button1, //TODO: Need to check this
		_2		= Button2,
		_3		= Button3,
		_4		= Button4,
		_5		= Button5,
	#endif

		Left	= _1,
		Right	= _2,
		Middle	= _3,
	};

//TODO: Xlib wheel delta
#if CLV_PLATFORM_WINDOWS
	#define CLV_WHEEL_DELTA WHEEL_DELTA
#elif CLV_PLATFORM_LINUX
	#define CLV_WHEEL_DELTA 0
#endif

	inline std::ostream& operator <<(std::ostream& os, MouseButton button){
		return os << static_cast<int>(button);
	}
}