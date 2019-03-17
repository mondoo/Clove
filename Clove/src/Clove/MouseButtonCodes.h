#pragma once

namespace clv{
	enum class MouseButton{
		_None = 0,

		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		Last	= _8,
		Left	= _1,
		Right	= _2,
		Middle	= _3,
	};

	namespace MouseButtonCodeHelpers{
		int CloveToGLFW(MouseButton button);
		MouseButton GLFWToClove(int button);
	}

	inline std::ostream& operator <<(std::ostream& os, MouseButton button){
		return os << static_cast<int>(button);
	}
}