#pragma once

#include "Membrane/EditorTypes.hpp"

namespace membrane {
    /**
     * @brief Allows the editor to easily access reflected types.
     */
public
    ref class ReflectionHelper {
        //FUNCTIONS
    public:
        static System::Collections::Generic::List<EditorTypeInfo ^> ^ getAvailableTypes();
        static EditorTypeInfo ^ getInfoForType(System::String ^ typeName);
    };
}