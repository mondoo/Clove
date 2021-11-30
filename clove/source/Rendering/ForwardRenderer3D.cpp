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

        graphicsQueue = ghaFactory->createGraphicsQueue(CommandQueueDescriptor{ .flags = QueueFlags::ReuseBuffers }).getValue();
        computeQueue  = ghaFactory->createComputeQueue(CommandQueueDescriptor{ .flags = QueueFlags::ReuseBuffers }).getValue();
        transferQueue = ghaFactory->createTransferQueue(CommandQueueDescriptor{ .flags = QueueFlags::ReuseBuffers }).getValue();

        for(size_t i{ 0 }; i < maxFramesInFlight + 1; ++i) {
            frameCaches.emplace_back(ghaFactory, graphicsQueue.get(), computeQueue.get(), transferQueue.get());
        }

        renderTargetPropertyChangedBeginHandle = this->renderTarget->onPropertiesChangedBegin.bind(&ForwardRenderer3D::resetGraphCaches, this);

        //Create semaphores for frame synchronisation
        for(auto &skinningFinishedSemaphore : skinningFinishedSemaphores) {
            skinningFinishedSemaphore = ghaFactory->createSemaphore().getValue();
        }
        for(auto &renderFinishedSemaphore : renderFinishedSemaphores) {
            renderFinishedSemaphore = ghaFactory->createSemaphore().getValue();
        }
        for(auto &imageAvailableSemaphore : imageAvailableSemaphores) {
            imageAvailableSemaphore = ghaFactory->createSemaphore().getValue();
        }

        //Create fences to wait for images in flight
        for(auto &fence : framesInFlight) {
            fence = ghaFactory->createFence({ true }).getValue();
        }

        //Create the geometry passes this renderer supports
        geometryPasses[GeometryPass::getId<SkinningPass>()]         = std::make_unique<SkinningPass>();//NOTE: This needs to be first (or early) as it modifies the vertex buffer.
        geometryPasses[GeometryPass::getId<ForwardColourPass>()]    = std::make_unique<ForwardColourPass>();
        geometryPasses[GeometryPass::getId<DirectionalLightPass>()] = std::make_unique<DirectionalLightPass>();
        //geometryPasses[GeometryPass::getId<PointLightPass>()]       = std::make_unique<PointLightPass>();

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

        for(auto &&[id, pass] : geometryPasses) {
            pass->flushJobs();
        }
    }

    void ForwardRenderer3D::submitMesh(MeshInfo meshInfo) {
        currentFrameData.meshes.push_back(std::move(meshInfo));
    }

    void ForwardRenderer3D::submitCamera(mat4f const view, mat4f const projection, vec3f const position) {
        currentFrameData.viewData.view       = view;
        currentFrameData.viewData.projection = projection;

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

        //Shadows
        std::vector<RgBufferId> directionalLightSpaceBuffers{};
        std::vector<RgBufferId> pointLightSpaceBuffers{};

        directionalLightSpaceBuffers.reserve(currentFrameData.numLights.numDirectional);
        for(int32_t i{ 0 }; i < currentFrameData.numLights.numDirectional; ++i) {
            RgBufferId lightSpaceBuffer{ renderGraph.createBuffer(sizeof(mat4f)) };
            renderGraph.writeToBuffer(lightSpaceBuffer, &currentFrameData.directionalShadowTransforms[i], 0, sizeof(mat4f));
            directionalLightSpaceBuffers.push_back(lightSpaceBuffer);
        }

        pointLightSpaceBuffers.reserve(currentFrameData.numLights.numPoint);
        for(int32_t i{ 0 }; i < currentFrameData.numLights.numPoint; ++i) {
            //TODO
            // RgBufferId lightSpaceBuffer{ renderGraph.createBuffer(sizeof(mat4f)) };
            // renderGraph.writeToBuffer(lightSpaceBuffer, &currentFrameData.pointShadowTransforms[i], 0, sizeof(mat4f));
        }

        //Images might not be going into the shadow pass but will always going into the lighting pass so we initialise them as ShaderReadOnlyOptimal
        RgImageId directionalShadowMap{ renderGraph.createImage(GhaImage::Type::_2D, GhaImage::Format::D32_SFLOAT, { shadowMapSize, shadowMapSize }, MAX_LIGHTS) };
        RgImageId pointShadowMap{ renderGraph.createImage(GhaImage::Type::Cube, GhaImage::Format::D32_SFLOAT, { shadowMapSize, shadowMapSize }, MAX_LIGHTS) };
        RgSampler shadowMaplSampler{ renderGraph.createSampler(GhaSampler::Descriptor{
            .minFilter        = GhaSampler::Filter::Linear,
            .magFilter        = GhaSampler::Filter::Linear,
            .addressModeU     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeV     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeW     = GhaSampler::AddressMode::ClampToBorder,
            .enableAnisotropy = false,
        }) };

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

        //Job info
        std::vector<GeometryPass::Job> geometryJobs{ currentFrameData.meshes.size() };
        float constexpr anisotropy{ 16.0f };
        for(size_t i{ 0 }; auto const &meshInfo : currentFrameData.meshes) {
            auto const &mesh{ meshInfo.mesh };

            //Uniforms
            RgBufferId modelBuffer{ renderGraph.createBuffer(sizeof(ModelData)) };
            RgBufferId colourBuffer{ renderGraph.createBuffer(sizeof(vec4f)) };

            ModelData const modelData{
                .model                 = meshInfo.transform,
                .inverseTransposeModel = inverse(transpose(meshInfo.transform)),
            };
            vec4f const colourData{ meshInfo.material->getColour() };

            size_t const modelBufferSize{ sizeof(modelData) };
            size_t const colourBufferSize{ sizeof(colourData) };

            renderGraph.writeToBuffer(modelBuffer, &modelData, 0, sizeof(modelData));
            renderGraph.writeToBuffer(colourBuffer, &colourData, 0, sizeof(colourData));

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

            //Animation
            size_t const matrixPaletteSize{ sizeof(mat4f) * MAX_JOINTS };
            RgBufferId matrixPalette{ renderGraph.createBuffer(matrixPaletteSize) };
            renderGraph.writeToBuffer(matrixPalette, meshInfo.matrixPalet.data(), 0, matrixPaletteSize);

            geometryJobs[i] = GeometryPass::Job{
                .vertexBuffer      = renderGraph.createBuffer(mesh->getCombinedBuffer(), mesh->getVertexOffset(), mesh->getVertexBufferSize()),
                .indexBuffer       = renderGraph.createBuffer(mesh->getCombinedBuffer(), mesh->getIndexOffset(), mesh->getIndexBufferSize()),
                .vertexCount       = mesh->getVertexCount(),
                .indexCount        = mesh->getIndexCount(),
                .vertexBufferSize  = mesh->getVertexBufferSize(),
                .modelBuffer       = modelBuffer,
                .colourBuffer      = colourBuffer,
                .modelBufferSize   = modelBufferSize,
                .colourBufferSize  = colourBufferSize,
                .diffuseTexture    = diffuseTexture,
                .specularTexture   = specularTexture,
                .materialSampler   = materialSampler,
                .matrixPalette     = matrixPalette,
                .matrixPaletteSize = matrixPaletteSize,
            };

            for(auto id : meshInfo.geometryPassIds) {
                if(geometryPasses.contains(id)) {
                    geometryPasses.at(id)->addJob(&geometryJobs[i]);
                }
            }

            ++i;
        }

        //Execute passes
        GeometryPass::PassData const passData{
            .renderTarget                 = renderTargetImage,
            .depthTarget                  = depthTargetImage,
            .renderTargetSize             = renderTarget->getSize(),
            .directionalShadowMap         = directionalShadowMap,
            .pointShadowMap               = pointShadowMap,
            .shadowMaplSampler            = shadowMaplSampler,
            .directionalLightCount        = currentFrameData.numLights.numDirectional,
            .pointLightCount              = currentFrameData.numLights.numPoint,
            .directionalLightSpaceBuffers = std::move(directionalLightSpaceBuffers),
            .pointLightSpaceBuffers       = std::move(pointLightSpaceBuffers),
            .viewUniformBuffer            = viewUniformBuffer,
            .viewDataSize                 = viewDataSize,
            .viewPositionSize             = viewPositionSize,
            .viewDataOffset               = viewDataOffset,
            .viewPositionOffset           = viewPositionOffset,
            .lightsUnfiromBuffer          = lightsUnfiromBuffer,
            .numLightsSize                = numLightsSize,
            .dirShadowTransformsSize      = dirShadowTransformsSize,
            .lightsSize                   = lightsSize,
            .numLightsOffset              = numLightsOffset,
            .dirShadowTransformsOffset    = dirShadowTransformsOffset,
            .lightsOffset                 = lightsOffset,
        };

        for(auto &&[id, pass] : geometryPasses) {
            pass->execute(renderGraph, passData);
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

    vec2ui ForwardRenderer3D::getRenderTargetSize() const {
        return renderTarget->getSize();
    }

    void ForwardRenderer3D::resetGraphCaches() {
        ghaDevice->waitForIdleDevice();

        for(auto &cache : frameCaches) {
            cache.clear();
        }
    }
}
