#pragma once

namespace garlic::clove {
    enum class PipelineStage {
        //External Stages
        Top,

        //Transfer stages
        Transfer,

        //Compute stages
        //...

        //Graphics stages
        PixelShader,
        EarlyPixelTest,
        ColourAttachmentOutput,
    };
}