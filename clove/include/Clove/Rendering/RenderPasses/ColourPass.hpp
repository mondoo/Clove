#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace garlic::clove {
    class GhaFactory;
    class GhaPipelineObject;
    class GhaRenderPass;
}

//TODO: Rename to lightingpass

namespace garlic::clove {
    class ColourPass : public GeometryPass {
        //VARIABLES
    private:
        std::unordered_map<DescriptorSetSlots, std::shared_ptr<garlic::clove::GhaDescriptorSetLayout>> descriptorSetLayouts;
        
        std::unique_ptr<GhaPipelineObject> staticMeshPipeline;
        std::unique_ptr<GhaPipelineObject> animatedMeshPipeline;

        //FUNCTIONS
    public:
        //TODO: Ctors
        ColourPass(GhaFactory &ghaFactory); //Takes an attachment descriptor for the output
        ~ColourPass();

        void recordPass(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) override;
    };
}