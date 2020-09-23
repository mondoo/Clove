#pragma once

#ifdef EXPORT
    #define SCAPE_DLL __declspec(dllexport)
#else
    #define SCAPE_DLL __declspec(dllimport)
#endif

extern "C" {
SCAPE_DLL void startEngine();

SCAPE_DLL void runEngine();
}