#pragma once

namespace clv::gfx {
    /**
     * @brief A Shader represents a small piece of code that runs on the GPU
     */
    class Shader {
        //TYPES
    public:
        enum class Stage {
            Vertex,
            Pixel,
            //Geometry //TODO: Implement when feature flags can be checked
        };

        //FUNCTIONS
    public:
        virtual ~Shader() = default;
    };
}