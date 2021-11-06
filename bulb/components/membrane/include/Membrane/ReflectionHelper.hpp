#pragma once

namespace membrane {
    public ref class EditorTypeInfo {
        //VARIABLES
    public:
        System::String ^ typeName { nullptr };
        System::String ^ displayName { nullptr };
        System::Collections::Generic::List<EditorTypeInfo ^> ^ members { nullptr };
    };

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