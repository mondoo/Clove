#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>

namespace clv::gfx {
    class GraphicsBuffer;
    class GraphicsImageView;
}

namespace blb::rnd {
    class Mesh;
    class Sprite;
}

namespace blb::rnd {
    class Renderer3D {
        //TYPES
    public:
        struct ComposedCameraData {
            clv::gfx::Viewport viewport;
            CameraRenderData bufferData;
            //std::shared_ptr<clv::gfx::RenderTarget> target;
        };

        //FUNCTIONS
    public:
        virtual void begin() = 0;

        virtual void submitPrimitive(const std::shared_ptr<clv::gfx::GraphicsBuffer>& vertexBuffer, const std::shared_ptr<clv::gfx::GraphicsBuffer>& indexBuffer, const clv::mth::mat4f& transform) = 0;
        virtual void submitQuad(const clv::mth::mat4f& transform, const clv::mth::vec4f& colour)                                                                                                    = 0;
        virtual void submitQuad(const clv::mth::mat4f& transform, const std::shared_ptr<clv::gfx::GraphicsImageView>& texture)                                                                      = 0;

        virtual void submitStaticMesh(const std::shared_ptr<Mesh>& mesh)   = 0;
        virtual void submitAnimatedMesh(const std::shared_ptr<Mesh>& mesh) = 0;
        virtual void submitLight(const DirectionalLight& light)            = 0;
        virtual void submitLight(const PointLight& light)                  = 0;
        virtual void submitCamera(const ComposedCameraData& camera)        = 0;

        virtual void submitWidget(const std::shared_ptr<Sprite>& widget) = 0;
        virtual void submitText(const std::shared_ptr<Sprite>& text)     = 0;

        virtual void end() = 0;
    };
}