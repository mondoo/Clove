#pragma once

#include "Clove/Platform/PlatformTypes.hpp"

#include <Clove/Maths/Vector.hpp>
#include <memory>

namespace garlic::clove {
    class Window;
}

namespace garlic::clove {
    class Platform {
        //FUNCTIONS
    public:
		/**
		 * @details Performs any platform specific initialisation that might be required.
		 */
		static void initialise();
		
        static std::unique_ptr<Window> createWindow(WindowDescriptor const &descriptor);
    };
}
