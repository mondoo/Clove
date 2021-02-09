#include <X11/Xlib.h>
#include <X11/Xutil.h>

//Undefine definitions brought in from X11 to prevent compile errors
#undef None
#undef Success

//Global display as we're only ever going to have one connection
inline Display *display{ nullptr };