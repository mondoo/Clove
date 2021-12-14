#include "Clove/Rendering/RenderPasses/PointLightPass.hpp"

#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"
#include "Clove/Rendering/RenderingConstants.hpp"
#include "Clove/Rendering/Vertex.hpp"

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char meshcubeshadowmap_v[];
extern "C" const size_t meshcubeshadowmap_vLength;
extern "C" const char meshcubeshadowmap_p[];
extern "C" const size_t meshcubeshadowmap_pLength;

namespace clove {
    PointLightPass::PointLightPass() = default;

    PointLightPass::PointLightPass(PointLightPass &&other) noexcept = default;

    PointLightPass &PointLightPass::operator=(PointLightPass &&other) noexcept = default;

    PointLightPass::~PointLightPass() = default;

    GeometryPass::Id PointLightPass::getId() const {
        return getIdOf<PointLightPass>();
    }

    void PointLightPass::execute(RenderGraph &renderGraph, PassData const &passData) {
        //TODO: Cache instead of making every frame
        std::unordered_map<std::string, std::string> shaderIncludes;
        shaderIncludes["Constants.glsl"] = { constants, constantsLength };

        //NOTE: This relies on the light space buffers being built in the same way inside the renderer.
        uint32_t constexpr cubeFaces{ 6 };
        for(size_t i{ 0 }; i < passData.pointLightCount; ++i) {
            for(size_t j{ 0 }; j < cubeFaces; ++j) {
                RgRenderPass::Descriptor passDescriptor{
                    .vertexShader     = renderGraph.createShader({ meshcubeshadowmap_v, meshcubeshadowmap_vLength }, shaderIncludes, "Mesh cube shadow map (vertex)", GhaShader::Stage::Vertex),
                    .pixelShader      = renderGraph.createShader({ meshcubeshadowmap_p, meshcubeshadowmap_pLength }, shaderIncludes, "Mesh cube shadow map (pixel)", GhaShader::Stage::Pixel),
                    .vertexInput      = Vertex::getInputBindingDescriptor(),
                    .vertexAttributes = {
                        VertexAttributeDescriptor{
                            .format = VertexAttributeFormat::R32G32B32_SFLOAT,
                            .offset = offsetof(Vertex, position),
                        },
                    },
                    .viewportSize = { shadowMapSize, shadowMapSize },
                    .depthStencil = {
                        .loadOp     = LoadOperation::Clear,
                        .storeOp    = StoreOperation::Store,
                        .clearValue = DepthStencilValue{ .depth = 1.0f },
                        .imageView  = {
                            .image      = passData.pointShadowMap,
                            .arrayIndex = static_cast<uint32_t>((i * cubeFaces) + j),
                        },
                    }
                };
                RgPassId pointShadowPass{ renderGraph.createRenderPass(passDescriptor) };

                for(auto *job : getJobs()) {
                    renderGraph.addRenderSubmission(pointShadowPass, RgRenderPass::Submission{
                                                                         .vertexBuffer = job->vertexBuffer,
                                                                         .indexBuffer  = job->indexBuffer,
                                                                         .shaderUbos   = {
                                                                             RgBufferBinding{
                                                                                 .slot        = 0,//NOLINT
                                                                                 .buffer      = job->modelBuffer,
                                                                                 .size        = job->modelBufferSize,
                                                                                 .shaderStage = GhaShader::Stage::Vertex,
                                                                             },
                                                                             RgBufferBinding{
                                                                                 .slot        = 1,//NOLINT
                                                                                 .buffer      = passData.pointLightSpaceBuffers[(i * cubeFaces) + j],
                                                                                 .size        = sizeof(mat4f),
                                                                                 .shaderStage = GhaShader::Stage::Vertex,
                                                                             },
                                                                             RgBufferBinding{
                                                                                 .slot        = 2,//NOLINT
                                                                                 .buffer      = passData.pointLightBuffers[i],
                                                                                 .size        = passData.pointLightBufferSize,
                                                                                 .shaderStage = GhaShader::Stage::Pixel,
                                                                             },
                                                                         },
                                                                         .indexCount = job->indexCount,
                                                                     });
                }
            }
        }
    }
}
