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
#include "Clove/Maths/MathsTypes.hpp"
#include "Clove/Maths/Maths.hpp"
#include "Clove/Maths/MathsHelpers.hpp"

//Vendor headers


//Platform specific headers
#if CLV_PLATFORM_WINDOWS
	#include "Platform/Windows/CloveWindows.hpp"
#endif
