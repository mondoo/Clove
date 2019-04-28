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
	#endif

		Left	= _1,
		Right	= _2,
		Middle	= _3,
	};

#if CLV_PLATFORM_WINDOWS
	#define CLV_WHEEL_DELTA WHEEL_DELTA
#endif


	inline std::ostream& operator <<(std::ostream& os, MouseButton button){
		return os << static_cast<int>(button);
	}
}