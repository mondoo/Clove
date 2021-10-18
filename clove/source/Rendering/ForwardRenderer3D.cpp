#include "Clove/Rendering/ForwardRenderer3D.hpp"

#include "Clove/Application.hpp"
#include "Clove/Rendering/Camera.hpp"
#include "Clove/Rendering/Material.hpp"
#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"
#include "Clove/Rendering/RenderPasses/DirectionalLightPass.hpp"
#include "Clove/Rendering/RenderPasses/ForwardColourPass.hpp"
#include "Clove/Rendering/RenderPasses/PointLightPass.hpp"
#include "Clove/Rendering/RenderPasses/SkinningPass.hpp"
#include "Clove/Rendering/RenderTarget.hpp"
#include "Clove/Rendering/Renderables/Mesh.hpp"
#include "Clove/Rendering/RenderingHelpers.hpp"
#include "Clove/Rendering/RenderingLog.hpp"
#include "Clove/Rendering/Vertex.hpp"

#include <Clove/Graphics/GhaDescriptorSet.hpp>
#include <Clove/Graphics/GhaDevice.hpp>
#include <Clove/Graphics/GhaFactory.hpp>
#include <Clove/Graphics/GhaImageView.hpp>
#include <Clove/Graphics/Graphics.hpp>
#include <Clove/Platform/Window.hpp>
#include <algorithm>

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char ui_v[];
extern "C" const size_t ui_vLength;
extern "C" const char widget_p[];
extern "C" const size_t widget_pLength;
extern "C" const char font_p[];
extern "C" const size_t font_pLength;

//TEMP
extern "C" const char mesh_v[];
extern "C" const size_t mesh_vLength;
extern "C" const char mesh_p[];
extern "C" const size_t mesh_pLength;

extern "C" const char meshshadowmap_v[];
extern "C" const size_t meshshadowmap_vLength;
extern "C" const char meshshadowmap_p[];
extern "C" const size_t meshshadowmap_pLength;

namespace clove {
    ForwardRenderer3D::ForwardRenderer3D(GhaDevice *ghaDevice, std::unique_ptr<RenderTarget> renderTarget)
        : ghaDevice{ ghaDevice }
        , renderTarget{ std::move(renderTarget) }
        , globalCache{ ghaDevice->getGraphicsFactory() } {
        GhaFactory *ghaFactory{ ghaDevice->getGraphicsFactory() };

        size_t const maxImages{ this->renderTarget->getImages().size() };
        maxFramesInFlight = std::max(static_cast<size_t>(1), maxImages - 1);

        skinningFinishedSemaphores.resize(maxFramesInFlight);
        renderFinishedSemaphores.resize(maxFramesInFlight);
        imageAvailableSemaphores.resize(maxFramesInFlight);
        framesInFlight.resize(maxFramesInFlight);
        imagesInFlight.resize(maxImages);

        shaderIncludes["Constants.glsl"] = { constants, constantsLength };

        graphicsQueue = ghaFactory->createGraphicsQueue(CommandQueueDescriptor{ .flags = QueueFlags::ReuseBuffers }).getValue();
        computeQueue  = ghaFactory->createComputeQueue(CommandQueueDescriptor{ .flags = QueueFlags::ReuseBuffers }).getValue();
        transferQueue = ghaFactory->createTransferQueue(CommandQueueDescriptor{ .flags = QueueFlags::ReuseBuffers }).getValue();

        for(size_t i{ 0 }; i < maxFramesInFlight + 1; ++i) {
            frameCaches.emplace_back(ghaFactory, graphicsQueue.get(), computeQueue.get(), transferQueue.get());
        }

        renderTargetPropertyChangedBeginHandle = this->renderTarget->onPropertiesChangedBegin.bind(&ForwardRenderer3D::resetGraphCaches, this);

        //Create semaphores for frame synchronisation
        for(auto &skinningFinishedSemaphore : skinningFinishedSemaphores) {
            skinningFinishedSemaphore = *ghaFactory->createSemaphore();
        }
        for(auto &renderFinishedSemaphore : renderFinishedSemaphores) {
            renderFinishedSemaphore = *ghaFactory->createSemaphore();
        }
        for(auto &imageAvailableSemaphore : imageAvailableSemaphores) {
            imageAvailableSemaphore = *ghaFactory->createSemaphore();
        }

        //Create fences to wait for images in flight
        for(auto &fence : framesInFlight) {
            fence = *ghaFactory->createFence({ true });
        }

#if 0
        std::vector<Vertex> const uiVertices{
            Vertex{
                .position = { 0.0f, 1.0f, 0.0f },
                .texCoord = { 0.0f, 0.0f },
            },
            Vertex{
                .position = { 1.0f, 1.0f, 0.0f },
                .texCoord = { 1.0f, 0.0f },
            },
            Vertex{
                .position = { 0.0f, 0.0f, 0.0f },
                .texCoord = { 0.0f, 1.0f },
            },
            Vertex{
                .position = { 1.0f, 0.0f, 0.0f },
                .texCoord = { 1.0f, 1.0f },
            },
        };

        std::vector<uint16_t> const uiIndices{
            0,
            2,
            3,
            0,
            3,
            1,
        };

        uiMesh = std::make_unique<Mesh>(uiVertices, uiIndices);
#endif
    }

    ForwardRenderer3D::ForwardRenderer3D(ForwardRenderer3D &&other) noexcept = default;

    ForwardRenderer3D &ForwardRenderer3D::operator=(ForwardRenderer3D &&other) noexcept = default;

    ForwardRenderer3D::~ForwardRenderer3D() {
        //Wait for an idle device before implicitly destructing the render graph caches.
        //This prevents issues when trying to destroy objects while they might still be in use.
        ghaDevice->waitForIdleDevice();
    }

    void ForwardRenderer3D::begin() {
        currentFrameData.meshes.clear();
#if 0
        currentFrameData.widgets.clear();
        currentFrameData.text.clear();
#endif

        currentFrameData.numLights.numDirectional = 0;
        currentFrameData.numLights.numPoint       = 0;
    }

    void ForwardRenderer3D::submitMesh(MeshInfo meshInfo, std::set<GeometryPass::Id> const &geometryPassIds) {
        currentFrameData.meshes.push_back(std::move(meshInfo));
#if 0
        for(auto id : geometryPassIds) {
            geometryPasses[id]->addJob({ currentFrameData.meshes.size() - 1, currentFrameData.meshes.back().mesh });
        }
#endif
    }

    void ForwardRenderer3D::submitCamera(Camera const &camera, vec3f position) {
        currentFrameData.viewData.view       = camera.getView();
        currentFrameData.viewData.projection = camera.getProjection();

        currentFrameData.viewPosition = position;
    }

    void ForwardRenderer3D::submitLight(DirectionalLight const &light) {
        uint32_t const lightIndex{ currentFrameData.numLights.numDirectional++ };

        currentFrameData.lights.directionalLights[lightIndex]    = light.data;
        currentFrameData.directionalShadowTransforms[lightIndex] = light.shadowTransform;
    }

    void ForwardRenderer3D::submitLight(PointLight const &light) {
        uint32_t const lightIndex{ currentFrameData.numLights.numPoint++ };

        currentFrameData.lights.pointLights[lightIndex]    = light.data;
        currentFrameData.pointShadowTransforms[lightIndex] = light.shadowTransforms;
    }

    void ForwardRenderer3D::submitWidget(std::shared_ptr<GhaImageView> const &widget, mat4f const modelProjection) {
#if 0
        currentFrameData.widgets.emplace_back(widget, modelProjection);
#endif
    }

    void ForwardRenderer3D::submitText(std::shared_ptr<GhaImageView> const &text, mat4f const modelProjection) {
#if 0
        currentFrameData.text.emplace_back(text, modelProjection);
#endif
    }

    void ForwardRenderer3D::end() {
        framesInFlight[currentFrame]->wait();

        //Aquire the next available image from the render target
        Expected<uint32_t, std::string> const result{ renderTarget->aquireNextImage(imageAvailableSemaphores[currentFrame].get()) };
        if(!result.hasValue()) {
            CLOVE_LOG(CloveRendering, LogLevel::Debug, result.getError());
            return;
        }

        size_t const imageIndex{ result.getValue() };

        //Check if we're already using the image. If so, wait
        if(imagesInFlight[imageIndex] != nullptr) {
            imagesInFlight[imageIndex]->wait();
        }
        imagesInFlight[imageIndex] = framesInFlight[currentFrame].get();

        framesInFlight[currentFrame]->reset();

        size_t const minUboOffsetAlignment{ ghaDevice->getLimits().minUniformBufferOffsetAlignment };

        //Build the render graph
        RenderGraph renderGraph{ frameCaches[imageIndex], globalCache };

        RgImageId renderTargetImage{ renderGraph.createImage(renderTarget->getImages()[imageIndex]) };
        renderGraph.registerGraphOutput(renderTargetImage);

        RgImageId depthTargetImage{ renderGraph.createImage(GhaImage::Type::_2D, GhaImage::Format::D32_SFLOAT, renderTarget->getSize()) };// TODO: This will probably be a manually created image.

        //View uniform buffer
        size_t const viewDataSize{ sizeof(currentFrameData.viewData) };
        size_t const viewPositionSize{ sizeof(currentFrameData.viewPosition) };

        size_t const viewDataOffset{ 0 };
        size_t const viewPositionOffset{ viewDataOffset + viewDataSize + (minUboOffsetAlignment - ((viewDataOffset + viewDataSize) % minUboOffsetAlignment)) };

        RgBufferId viewUniformBuffer{ renderGraph.createBuffer(viewPositionOffset + viewPositionSize) };

        renderGraph.writeToBuffer(viewUniformBuffer, &currentFrameData.viewData, viewDataOffset, viewDataSize);
        renderGraph.writeToBuffer(viewUniformBuffer, &currentFrameData.viewPosition, viewPositionOffset, viewPositionSize);

        //Lights uniform buffer
        size_t const numLightsSize{ sizeof(currentFrameData.numLights) };
        size_t const dirShadowTransformsSize{ sizeof(currentFrameData.directionalShadowTransforms) };
        size_t const lightsSize{ sizeof(currentFrameData.lights) };

        size_t const numLightsOffset{ 0 };
        size_t const dirShadowTransformsOffset{ numLightsOffset + numLightsSize + (minUboOffsetAlignment - ((numLightsOffset + numLightsSize) % minUboOffsetAlignment)) };
        size_t const lightsOffset{ dirShadowTransformsOffset + dirShadowTransformsSize + (minUboOffsetAlignment - ((dirShadowTransformsOffset + dirShadowTransformsSize) % minUboOffsetAlignment)) };

        RgBufferId lightsUnfiromBuffer{ renderGraph.createBuffer(lightsOffset + lightsSize) };

        renderGraph.writeToBuffer(lightsUnfiromBuffer, &currentFrameData.numLights, numLightsOffset, numLightsSize);
        renderGraph.writeToBuffer(lightsUnfiromBuffer, &currentFrameData.directionalShadowTransforms, dirShadowTransformsOffset, dirShadowTransformsSize);
        renderGraph.writeToBuffer(lightsUnfiromBuffer, &currentFrameData.lights, lightsOffset, lightsSize);

        //Mesh buffers
        struct MeshBuffers {
            RgBufferId vertexBuffer{};
            RgBufferId indexBuffer{};

            RgBufferId modelBuffer{};
            RgBufferId colourBuffer{};

            size_t modelBufferSize{};
            size_t colourBufferSize{};
        };
        std::vector<MeshBuffers> meshBuffers{};

        for(auto const &meshInfo : currentFrameData.meshes) {
            auto const &mesh{ meshInfo.mesh };

            MeshBuffers bufferData{};

            //Mesh data
            bufferData.vertexBuffer = renderGraph.createBuffer(mesh->getCombinedBuffer(), mesh->getVertexOffset(), mesh->getVertexBufferSize());
            bufferData.indexBuffer  = renderGraph.createBuffer(mesh->getCombinedBuffer(), mesh->getIndexOffset(), mesh->getIndexBufferSize());

            //Uniform buffer
            bufferData.modelBuffer  = renderGraph.createBuffer(sizeof(ModelData));
            bufferData.colourBuffer = renderGraph.createBuffer(sizeof(vec4f));

            ModelData const modelData{
                .model                 = meshInfo.transform,
                .inverseTransposeModel = inverse(transpose(meshInfo.transform)),
            };
            vec4f const colourData{ meshInfo.material->getColour() };

            bufferData.modelBufferSize  = sizeof(modelData);
            bufferData.colourBufferSize = sizeof(colourData);

            renderGraph.writeToBuffer(bufferData.modelBuffer, &modelData, 0, sizeof(modelData));
            renderGraph.writeToBuffer(bufferData.colourBuffer, &colourData, 0, sizeof(colourData));

            meshBuffers.push_back(std::move(bufferData));
        }

        //Images might not going into the shadow pass but will always going into the lighting pass so we initialise them as ShaderReadOnlyOptimal
        RgImageId directionalShadowMap{ renderGraph.createImage(GhaImage::Type::_2D, GhaImage::Format::D32_SFLOAT, { shadowMapSize, shadowMapSize }, MAX_LIGHTS) };
        RgImageId pointShadowMap{ renderGraph.createImage(GhaImage::Type::Cube, GhaImage::Format::D32_SFLOAT, { shadowMapSize, shadowMapSize }, MAX_LIGHTS) };

        //DIRECTIONAL SHADOWS
        for(size_t i{ 0 }; i < currentFrameData.numLights.numDirectional; ++i) {
            //NOTE: Need this as a separate thing otherwise there is an internal compiler error. I think it's because of the clearValue variant
            RgRenderPass::Descriptor passDescriptor{
                .vertexShader = renderGraph.createShader({ meshshadowmap_v, meshshadowmap_vLength }, shaderIncludes, "Mesh (vertex)", GhaShader::Stage::Vertex),
                .pixelShader  = renderGraph.createShader({ meshshadowmap_p, meshshadowmap_pLength }, shaderIncludes, "Mesh (pixel)", GhaShader::Stage::Pixel),
                .viewportSize = { shadowMapSize, shadowMapSize },
                .depthStencil = {
                    .loadOp           = LoadOperation::Clear,
                    .storeOp          = StoreOperation::Store,
                    .clearValue       = DepthStencilValue{ .depth = 1.0f },
                    .target           = directionalShadowMap,
                    .targetArrayIndex = static_cast<uint32_t>(i),
                    .targetArrayCount = 1,
                }
            };
            RgPassId directionalShadowPass{ renderGraph.createRenderPass(passDescriptor) };

            for(size_t meshIndex{ 0 }; auto const &meshInfo : currentFrameData.meshes) {
                auto const &mesh{ meshInfo.mesh };

                RgBufferId lightSpaceBuffer{ renderGraph.createBuffer(sizeof(mat4f)) };

                renderGraph.writeToBuffer(lightSpaceBuffer, &currentFrameData.directionalShadowTransforms[i], 0, sizeof(mat4f));

                renderGraph.addRenderSubmission(directionalShadowPass, RgRenderPass::Submission{
                                                                           .vertexBuffer = meshBuffers[meshIndex].vertexBuffer,
                                                                           .indexBuffer  = meshBuffers[meshIndex].indexBuffer,
                                                                           .shaderUbos   = {
                                                                               RgBufferBinding{
                                                                                   .slot        = 0,
                                                                                   .buffer      = meshBuffers[meshIndex].modelBuffer,
                                                                                   .offset      = 0,
                                                                                   .size        = meshBuffers[meshIndex].modelBufferSize,
                                                                                   .shaderStage = GhaShader::Stage::Vertex,
                                                                               },
                                                                               RgBufferBinding{
                                                                                   .slot        = 1,
                                                                                   .buffer      = lightSpaceBuffer,
                                                                                   .offset      = 0,
                                                                                   .size        = sizeof(mat4f),
                                                                                   .shaderStage = GhaShader::Stage::Vertex,
                                                                               },
                                                                           },
                                                                           .indexCount = mesh->getIndexCount(),
                                                                       });

                ++meshIndex;
            }
        }

        //FINAL COLOUR
        //NOTE: Need this as a separate thing otherwise there is an internal compiler error. I think it's because of the clearValue variant
        RgRenderPass::Descriptor passDescriptor{
            .vertexShader  = renderGraph.createShader({ mesh_v, mesh_vLength }, shaderIncludes, "Mesh (vertex)", GhaShader::Stage::Vertex),
            .pixelShader   = renderGraph.createShader({ mesh_p, mesh_pLength }, shaderIncludes, "Mesh (pixel)", GhaShader::Stage::Pixel),
            .viewportSize  = renderTarget->getSize(),
            .renderTargets = {
                RgRenderTargetBinding{
                    .loadOp     = LoadOperation::Clear,
                    .storeOp    = StoreOperation::Store,
                    .clearValue = ColourValue{ 0.0f, 0.0, 0.0f, 1.0f },
                    .target     = renderTargetImage,
                },
            },
            .depthStencil = {
                .loadOp     = LoadOperation::Clear,
                .storeOp    = StoreOperation::DontCare,
                .clearValue = DepthStencilValue{ .depth = 1.0f },
                .target     = depthTargetImage,
            },
        };
        RgPassId colourPass{ renderGraph.createRenderPass(passDescriptor) };

        RgSampler shadowMaplSampler{ renderGraph.createSampler(GhaSampler::Descriptor{
            .minFilter        = GhaSampler::Filter::Linear,
            .magFilter        = GhaSampler::Filter::Linear,
            .addressModeU     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeV     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeW     = GhaSampler::AddressMode::ClampToBorder,
            .enableAnisotropy = false,
        }) };

        for(size_t meshIndex{ 0 }; auto const &meshInfo : currentFrameData.meshes) {
            auto const &mesh{ meshInfo.mesh };
            float constexpr anisotropy{ 16.0f };

            //Textures
            RgImageId diffuseTexture{ renderGraph.createImage(meshInfo.material->getDiffuseImage()) };
            RgImageId specularTexture{ renderGraph.createImage(meshInfo.material->getSpecularImage()) };
            RgSampler materialSampler{ renderGraph.createSampler(GhaSampler::Descriptor{
                .minFilter        = GhaSampler::Filter::Linear,
                .magFilter        = GhaSampler::Filter::Linear,
                .addressModeU     = GhaSampler::AddressMode::Repeat,
                .addressModeV     = GhaSampler::AddressMode::Repeat,
                .addressModeW     = GhaSampler::AddressMode::Repeat,
                .enableAnisotropy = true,
                .maxAnisotropy    = anisotropy,
            }) };

            renderGraph.addRenderSubmission(colourPass, RgRenderPass::Submission{
                                                            .vertexBuffer = meshBuffers[meshIndex].vertexBuffer,
                                                            .indexBuffer  = meshBuffers[meshIndex].indexBuffer,
                                                            .shaderUbos   = {
                                                                RgBufferBinding{
                                                                    .slot        = 0,
                                                                    .buffer      = meshBuffers[meshIndex].modelBuffer,
                                                                    .offset      = 0,
                                                                    .size        = meshBuffers[meshIndex].modelBufferSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 1,
                                                                    .buffer      = viewUniformBuffer,
                                                                    .offset      = viewDataOffset,
                                                                    .size        = viewDataSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 2,
                                                                    .buffer      = lightsUnfiromBuffer,
                                                                    .offset      = numLightsOffset,
                                                                    .size        = numLightsSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex | GhaShader::Stage::Pixel,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 3,
                                                                    .buffer      = lightsUnfiromBuffer,
                                                                    .offset      = dirShadowTransformsOffset,
                                                                    .size        = dirShadowTransformsSize,
                                                                    .shaderStage = GhaShader::Stage::Vertex,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 10,
                                                                    .buffer      = viewUniformBuffer,
                                                                    .offset      = viewPositionOffset,
                                                                    .size        = viewPositionSize,
                                                                    .shaderStage = GhaShader::Stage::Pixel,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 11,
                                                                    .buffer      = lightsUnfiromBuffer,
                                                                    .offset      = lightsOffset,
                                                                    .size        = lightsSize,
                                                                    .shaderStage = GhaShader::Stage::Pixel,
                                                                },
                                                                RgBufferBinding{
                                                                    .slot        = 12,
                                                                    .buffer      = meshBuffers[meshIndex].colourBuffer,
                                                                    .offset      = 0,
                                                                    .size        = meshBuffers[meshIndex].colourBufferSize,
                                                                    .shaderStage = GhaShader::Stage::Pixel,
                                                                },
                                                            },
                                                            .shaderImages = {
                                                                RgImageBinding{
                                                                    .slot       = 4,
                                                                    .image      = diffuseTexture,
                                                                    .arrayIndex = 0,
                                                                    .arrayCount = 1,
                                                                },
                                                                RgImageBinding{
                                                                    .slot       = 5,
                                                                    .image      = specularTexture,
                                                                    .arrayIndex = 0,
                                                                    .arrayCount = 1,
                                                                },
                                                                RgImageBinding{
                                                                    .slot       = 7,
                                                                    .image      = directionalShadowMap,
                                                                    .arrayIndex = 0,
                                                                    .arrayCount = MAX_LIGHTS,
                                                                },
                                                                RgImageBinding{
                                                                    .slot       = 8,
                                                                    .image      = pointShadowMap,
                                                                    .arrayIndex = 0,
                                                                    .arrayCount = MAX_LIGHTS * cubeMapLayerCount,
                                                                },
                                                            },
                                                            .shaderSamplers = {
                                                                RgSamplerBinding{
                                                                    .slot    = 6,
                                                                    .sampler = materialSampler,
                                                                },
                                                                RgSamplerBinding{
                                                                    .slot    = 9,
                                                                    .sampler = shadowMaplSampler,
                                                                },
                                                            },
                                                            .indexCount = mesh->getIndexCount(),
                                                        });

            ++meshIndex;
        }

        renderGraph.execute(RenderGraph::ExecutionInfo{
            .waitSemaphores   = { { imageAvailableSemaphores[currentFrame].get(), PipelineStage::ColourAttachmentOutput } },
            .signalSemaphores = { renderFinishedSemaphores[currentFrame].get() },
            .signalFence      = framesInFlight[currentFrame].get(),
        });

        //Tell the render target to present the image we just submitted to
        renderTarget->present(imageIndex, { renderFinishedSemaphores[currentFrame].get() });

        //Advance to the next frame
        currentFrame = (currentFrame + 1) % maxFramesInFlight;
    }

    void ForwardRenderer3D::resetGraphCaches() {
        ghaDevice->waitForIdleDevice();

        for(auto &cache : frameCaches) {
            cache.clear();
        }
    }
}
