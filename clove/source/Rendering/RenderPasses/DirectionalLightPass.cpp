#include "Clove/Rendering/RenderPasses/DirectionalLightPass.hpp"

#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"
#include "Clove/Rendering/RenderGraph/RgPass.hpp"
#include "Clove/Rendering/RenderingConstants.hpp"
#include "Clove/Rendering/Vertex.hpp"

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char meshshadowmap_v[];
extern "C" const size_t meshshadowmap_vLength;
extern "C" const char meshshadowmap_p[];
extern "C" const size_t meshshadowmap_pLength;

namespace clove {
    DirectionalLightPass::DirectionalLightPass() = default;

    DirectionalLightPass::DirectionalLightPass(DirectionalLightPass &&other) noexcept = default;

    DirectionalLightPass &DirectionalLightPass::operator=(DirectionalLightPass &&other) noexcept = default;

    DirectionalLightPass::~DirectionalLightPass() = default;

    GeometryPass::Id DirectionalLightPass::getId() const {
        return getIdOf<DirectionalLightPass>();
    }
    
    void DirectionalLightPass::execute(RenderGraph &renderGraph, PassData const &passData) {
        //TODO: Cache instead of making every frame
        std::unordered_map<std::string, std::string> shaderIncludes;
        shaderIncludes["Constants.glsl"] = { constants, constantsLength };

        for(size_t i{ 0 }; i < passData.directionalLightCount; ++i) {
            //NOTE: Need this as a separate thing otherwise there is an internal compiler error. I think it's because of the clearValue variant
            RgRenderPass::Descriptor passDescriptor{
                .vertexShader     = renderGraph.createShader({ meshshadowmap_v, meshshadowmap_vLength }, shaderIncludes, "Mesh shadow map (vertex)", GhaShader::Stage::Vertex),
                .pixelShader      = renderGraph.createShader({ meshshadowmap_p, meshshadowmap_pLength }, shaderIncludes, "Mesh shadow map (pixel)", GhaShader::Stage::Pixel),
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
                        .image      = passData.directionalShadowMap,
                        .arrayIndex = static_cast<uint32_t>(i),
                        .arrayCount = 1,
                    },
                }
            };
            RgPassId directionalShadowPass{ renderGraph.createRenderPass(passDescriptor) };

            for(auto *job : getJobs()) {
                renderGraph.addRenderSubmission(directionalShadowPass, RgRenderPass::Submission{
                                                                           .vertexBuffer = job->vertexBuffer,
                                                                           .indexBuffer  = job->indexBuffer,
                                                                           .shaderUbos   = {
                                                                               RgBufferBinding{
                                                                                   .slot        = 0,
                                                                                   .buffer      = job->modelBuffer,
                                                                                   .offset      = 0,
                                                                                   .size        = job->modelBufferSize,
                                                                                   .shaderStage = GhaShader::Stage::Vertex,
                                                                               },
                                                                               RgBufferBinding{
                                                                                   .slot        = 1,
                                                                                   .buffer      = passData.directionalLightSpaceBuffers[i],
                                                                                   .offset      = 0,
                                                                                   .size        = sizeof(mat4f),
                                                                                   .shaderStage = GhaShader::Stage::Vertex,
                                                                               },
                                                                           },
                                                                           .indexCount = job->indexCount,
                                                                       });
            }
        }
    }
}
