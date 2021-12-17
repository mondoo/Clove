#pragma once

namespace membrane {
    public ref class EditorTypeInfo {
        //VARIABLES
    public:
        System::String ^typeName { nullptr };
        System::String ^displayName { nullptr };

        System::UInt32 offset{ 0 }; /**< Offset into the parent of this type. */

        System::Collections::Generic::List<EditorTypeInfo ^> ^members { nullptr };
        System::String ^value { nullptr }; /**< If this type is a leaf type (does not contain it's own members) then this will hold it's value.*/
    };

    public ref class AvailableTypeInfo {
        //VARIABLES
    public:
        System::String ^typeName { nullptr };
        System::String ^displayName { nullptr };
    };

    /**
     * @brief Allows the editor to easily access reflected types.
     */
    public ref class ReflectionHelper {
        //FUNCTIONS
    public:
        static System::Collections::Generic::List<AvailableTypeInfo ^> ^getAvailableTypes();
    };
}