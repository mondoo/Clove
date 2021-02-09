#include <X11/Xlib.h>
#include <X11/Xutil.h>

//Undefine definitions brought in from X11 to prevent compile errors
#undef None
#undef Success

//Global display as we're only ever going to have one connection
inline Display *display{ nullptr };
//Bit of a hack because of the Mouse is set up. Ideally we'll give the mouse the window the game is using
//but because we're only going to have one window now for we canget away with it. see LinuxMouse.cpp
inline ::Window window{ 0 };