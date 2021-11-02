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

    public ref class EditorTypeInfo {
        //VARIABLES
    public:
        System::String ^typeName {};
        System::String ^displayName {};
    };
    // clang-format on
}