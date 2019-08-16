#pragma once

//Generic headers
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>

//Data structures
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//Clove headers
#include "Clove/Core.hpp"
#include "Clove/Log.hpp"
#include "Clove/IntTypes.hpp"
#include "Clove/Maths/MathsTypes.hpp"
#include "Clove/Maths/Maths.hpp"
#include "Clove/Maths/MathsHelpers.hpp"

//Vendor headers
//...

//Platform specific headers
#if CLV_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	
	#include <Windows.h>
#elif CLV_PLATFORM_LINUX
	#define None
	
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>

	#undef None
#endif
