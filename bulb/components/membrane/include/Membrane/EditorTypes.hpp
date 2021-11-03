#pragma once

namespace membrane {
    // clang-format off
    public enum class ShapeType {
        Sphere,
        Cube
    };

    public value struct Vector3 {
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z)
            : x{ x }
            , y{ y }
            , z{ z } {
        }
    };

    public ref class TypeMemberInfo{
        //VARIABLES
    public:
        System::String ^typeName {};
        System::String ^displayName {};
    };

    public ref class EditorTypeInfo {
        //VARIABLES
    public:
        System::String ^typeName{ nullptr };
        System::String ^displayName{ nullptr };
        System::Collections::Generic::List<TypeMemberInfo ^> ^members{ nullptr };
    };
    // clang-format on
}