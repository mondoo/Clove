#pragma once

#if GARLIC_PLATFORM_WINDOWS
    #ifdef EXPORT
        #define SCAPE_DLL __declspec(dllexport)
    #else
        #define SCAPE_DLL __declspec(dllimport)
    #endif
#else
    #define SCAPE_DLL
#endif

namespace garlic::inline stem {
    class Application;
}

extern "C" {
SCAPE_DLL garlic::Application *application_create();
SCAPE_DLL void application_destroy(garlic::Application *app);

SCAPE_DLL void application_run(garlic::Application *app);
}