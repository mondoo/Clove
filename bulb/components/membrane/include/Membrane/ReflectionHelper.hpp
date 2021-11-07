#pragma once

namespace membrane {
    public ref class MemberInfo {
        //VARIABLES
    public:
        System::Int32 offset{ 0 }; /**< Offset into the whole memory for the enclosing type and not just local offset provided by reflection system. */
        System::Int32 size{ 0 };

        //FUNCTIONS
    public:
        MemberInfo(System::Int32 offset, System::Int32 size) 
            : offset{ offset }
            , size{ size }{
        }
    };

    public ref class EditorTypeInfo {
        //VARIABLES
    public:
        System::String ^ typeName { nullptr };
        System::String ^ displayName { nullptr };
        System::Collections::Generic::List<EditorTypeInfo ^> ^ members { nullptr };

        MemberInfo ^ memberInfo { nullptr }; /**< If this type is a member within another then this contains the offset and size within that member. */
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