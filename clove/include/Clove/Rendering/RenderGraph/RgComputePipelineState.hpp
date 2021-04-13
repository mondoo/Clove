#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"
#include "Clove/Rendering/RenderGraph/RgShader.hpp"

#include <Clove/Graphics/GhaComputePipelineObject.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <memory>

namespace garlic::clove {
    class RgComputePipelineState {
        //TYPES
    public:
        struct Descriptor {
            RgShader computeShader;
        };

        //VARIABLES
    private:
        GhaComputePipelineObject::Descriptor ghaDescriptor{};//TODO: Gets updated or is only set when this is created?

        //TODO: Keep some sort of internal ID for tracking

        //FUNCTIONS
    public:
        RgComputePipelineState();

    private:
        RgComputePipelineState(Descriptor descriptor);
    };
}