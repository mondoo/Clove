#pragma once

#include "Clove/Rendering/Proxies.hpp"

#include <vector>

namespace garlic::clove {
    class GhaDescriptorSet;
    class GhaGraphicsCommandBuffer;
    class Mesh;
}

namespace garlic::clove {
    /**
     * @brief A GeometryPass is a single graphics operation that is performed on a set of meshes.
     */
    class GeometryPass {
        //TYPES
    public:
        //TODO: Something like this? Passed into every pass, should only be global data (in frame scope)
        struct FrameData{
            std::shared_ptr<GhaDescriptorSet> viewDescriptorSet;
            std::shared_ptr<GhaDescriptorSet> lightingDescriptorSet;

            //TODO: Not have these in the framedata. There should be another way for a mesh to register itself to a pass (like a RenderTechnique that encompasses many passes)
            std::vector<std::shared_ptr<Mesh>> const &staticMeshes;
            std::vector<std::shared_ptr<Mesh>> const &animatedMeshes;
        };

        //TODO: Make the models a drawable/renderable? Then they can bind themselves?

        //FUNCTIONS
    public:
        virtual ~GeometryPass() = default;

        //TODO: Not perform a pass on every mesh. Only those that require it

        virtual void recordPass(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) = 0;
    };
}