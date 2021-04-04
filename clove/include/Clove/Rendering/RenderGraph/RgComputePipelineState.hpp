#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"

#include <Clove/Graphics/GhaComputePipelineObject.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <memory>
#include <vector>

namespace garlic::clove {
    class RgComputePipelineState {
        //TYPES
    public:
        struct Descriptor {
            std::shared_ptr<GhaShader> computeShader;
        };

        //VARIABLES
    private:
        GhaComputePipelineObject::Descriptor ghaDescriptor{};//TODO: Gets updated or is only set when this is created?

        //TODO: Keep some sort of internal ID for tracking

        //FUNCTIONS
    public:
        RgComputePipelineState();
        RgComputePipelineState(Descriptor descriptor);

        //TODO
    };
}