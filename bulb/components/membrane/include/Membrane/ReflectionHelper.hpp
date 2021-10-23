#pragma once

namespace membrane {
    //TODO: move to another file
public
    ref class TypeInfo {
        //VARIABLES
    public:
        System::String ^ name {};
    };

    /**
     * @brief Allows the editor to easily access reflected types.
     */
public
    ref class ReflectionHelper {
        //FUNCTIONS
    public:
        static System::Collections::Generic::List<TypeInfo ^> ^ getAvailableTypes();
    };
}