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
#include "Clove/Events/Delegate.hpp"

//Vendor headers
//...

//Platform specific headers
#if CLV_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOGDICAPMASKS
	#define NOVIRTUALKEYCODES
	#define NOWINMESSAGES
	#define NOWINSTYLES
	#define NOSYSMETRICS
	#define NOMENUS
	#define NOICONS
	#define NOKEYSTATES
	#define NOSYSCOMMANDS
	#define NORASTEROPS
	#define NOSHOWWINDOW
	#define OEMRESOURCE
	#define NOATOM
	#define NOCLIPBOARD
	#define NOCOLOR
	#define NOCTLMGR
	#define NODRAWTEXT
	#define NOGDI
	#define NOKERNEL
	#define NOUSER
	#define NONLS
	#define NOMB
	#define NOMEMMGR
	#define NOMETAFILE
	#define NOMINMAX
	#define NOMSG
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	#define NOTEXTMETRIC
	#define NOWH
	#define NOWINOFFSETS
	#define NOCOMM
	#define NOKANJI
	#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
	#define STRICT

	#include <Windows.h>
#elif CLV_PLATFORM_LINUX
	#define None
	
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>

	#undef None
#endif
