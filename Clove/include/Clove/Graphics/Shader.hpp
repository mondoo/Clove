#pragma once

#include <Root/Definitions.hpp>
#include <cinttypes>

namespace clv::gfx {
    /**
     * @brief A Shader represents a small piece of code that runs on the GPU
     */
    class Shader {
        //TYPES
    public:
        using StageType = uint16_t;
        enum class Stage : StageType {
            Vertex = 1 << 0,
            Pixel  = 1 << 1,
            //Geometry //TODO: Implement when feature flags can be checked
        };

        //FUNCTIONS
    public:
        virtual ~Shader() = default;
    };

    GARLIC_ENUM_BIT_FLAG_OPERATORS(Shader::Stage, Shader::StageType)
}