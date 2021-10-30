#include "Clove/Rendering/RenderPasses/ForwardColourPass.hpp"

#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"
#include "Clove/Rendering/RenderGraph/RgPass.hpp"
#include "Clove/Rendering/RenderingConstants.hpp"

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char mesh_v[];
extern "C" const size_t mesh_vLength;
extern "C" const char mesh_p[];
extern "C" const size_t mesh_pLength;

namespace clove {
    ForwardColourPass::ForwardColourPass() = default;

    ForwardColourPass::ForwardColourPass(ForwardColourPass &&other) noexcept = default;

    ForwardColourPass &ForwardColourPass::operator=(ForwardColourPass &&other) noexcept = default;

    ForwardColourPass::~ForwardColourPass() = default;

    void ForwardColourPass::execute(RenderGraph &renderGraph, PassData const &passData) {
        //TODO: Cache instead of making every frame
        std::unordered_map<std::string, std::string> shaderIncludes;
        shaderIncludes["Constants.glsl"] = { constants, constantsLength };

        //NOTE: Need this as a separate thing otherwise there is an internal compiler error. I think it's because of the clearValue variant
        RgRenderPass::Descriptor passDescriptor{
            .vertexShader  = renderGraph.createShader({ mesh_v, mesh_vLength }, shaderIncludes, "Mesh (vertex)", GhaShader::Stage::Vertex),
            .pixelShader   = renderGraph.createShader({ mesh_p, mesh_pLength }, shaderIncludes, "Mesh (pixel)", GhaShader::Stage::Pixel),
            .viewportSize  = passData.renderTargetSize,
            .renderTargets = {
                RgRenderTargetBinding{
                    .loadOp     = LoadOperation::Clear,
                    .storeOp    = StoreOperation::Store,
                    .clearValue = ColourValue{ 0.0f, 0.0, 0.0f, 1.0f },
                    .imageView  = {
                        .image = passData.renderTarget,
                    },
                },
            },
            .depthStencil = {
                .loadOp     = LoadOperation::Clear,
                .storeOp    = StoreOperation::DontCare,
                .clearValue = DepthStencilValue{ .depth = 1.0f },
                .imageView  = {
                    .image = passData.depthTarget,
                },
            },
        };
        RgPassId colourPass{ renderGraph.createRenderPass(passDescriptor) };

        for(auto &job : getJobs()) {
            renderGraph.addRenderSubmission(colourPass, RgRenderPass::Submission{
                                                            .vertexBuffer = job.vertexBuffer,
                                                            .indexBuffer  = job.indexBuffer,
                                                            .shaderUbos   = {
                                                                RgBufferBinding{
                                                                    .slot        = 0,
                                                                    .buffer      = job.modelBuffer,
                                                                    .offset      = 0,
                                                                    .size        = job.modelBufferSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 1,
                                                                    .buffer      = passData.viewUniformBuffer,
                                                                    .offset      = passData.viewDataOffset,
                                                                    .size        = passData.viewDataSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 2,
                                                                    .buffer      = passData.lightsUnfiromBuffer,
                                                                    .offset      = passData.numLightsOffset,
                                                                    .size        = passData.numLightsSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex | GhaShader::Stage::Pixel,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 3,
                                                                    .buffer      = passData.lightsUnfiromBuffer,
                                                                    .offset      = passData.dirShadowTransformsOffset,
                                                                    .size        = passData.dirShadowTransformsSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 10,
                                                                    .buffer      = passData.viewUniformBuffer,
                                                                    .offset      = passData.viewPositionOffset,
                                                                    .size        = passData.viewPositionSize,
                                                                    .shaderStage = GhaShader::Stage::Pixel,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 11,
                                                                    .buffer      = passData.lightsUnfiromBuffer,
                                                                    .offset      = passData.lightsOffset,
                                                                    .size        = passData.lightsSize,
                                                                    .shaderStage = GhaShader::Stage::Pixel,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 12,
                                                                    .buffer      = job.colourBuffer,
                                                                    .offset      = 0,
                                                                    .size        = job.colourBufferSize,
                                                                    .shaderStage = GhaShader::Stage::Pixel,
                                                                },
                                                            },
                                                            .shaderImages = {
                                                                RgImageBinding{
                                                                    .slot      = 4,
                                                                    .imageView = {
                                                                        .image = job.diffuseTexture,
                                                                    },
                                                                },
                                                                RgImageBinding{
                                                                    .slot      = 5,
                                                                    .imageView = {
                                                                        .image = job.specularTexture,
                                                                    },
                                                                },
                                                                RgImageBinding{
                                                                    .slot      = 7,
                                                                    .imageView = {
                                                                        .image      = passData.directionalShadowMap,
                                                                        .arrayIndex = 0,
                                                                        .arrayCount = MAX_LIGHTS,
                                                                    },
                                                                },
                                                                RgImageBinding{
                                                                    .slot      = 8,
                                                                    .imageView = {
                                                                        .image      = passData.pointShadowMap,
                                                                        .arrayIndex = 0,
                                                                        .arrayCount = MAX_LIGHTS * cubeMapLayerCount,
                                                                    },
                                                                },
                                                            },
                                                            .shaderSamplers = {
                                                                RgSamplerBinding{
                                                                    .slot    = 6,
                                                                    .sampler = job.materialSampler,
                                                                },
                                                                RgSamplerBinding{
                                                                    .slot    = 9,
                                                                    .sampler = passData.shadowMaplSampler,
                                                                },
                                                            },
                                                            .indexCount = job.indexCount,
                                                        });
        }
    }
}