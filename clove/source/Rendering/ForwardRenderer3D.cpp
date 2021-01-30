#include "Clove/Rendering/ForwardRenderer3D.hpp"

#include "Clove/Application.hpp"
#include "Clove/Rendering/Camera.hpp"
#include "Clove/Rendering/Material.hpp"
#include "Clove/Rendering/RenderPasses/ForwardColourPass.hpp"
#include "Clove/Rendering/RenderTarget.hpp"
#include "Clove/Rendering/Renderables/Mesh.hpp"
#include "Clove/Rendering/RenderingHelpers.hpp"
#include "Clove/Rendering/Vertex.hpp"

#include <Clove/Graphics/GhaDescriptorSet.hpp>
#include <Clove/Graphics/GhaImageView.hpp>
#include <Clove/Graphics/Graphics.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/Platform/Window.hpp>

using namespace garlic::clove;

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char staticmeshshadowmap_v[];
extern "C" const size_t staticmeshshadowmap_vLength;
extern "C" const char staticmeshcubeshadowmap_v[];
extern "C" const size_t staticmeshcubeshadowmap_vLength;

extern "C" const char animatedmeshshadowmap_v[];
extern "C" const size_t animatedmeshshadowmap_vLength;
extern "C" const char animatedmeshcubeshadowmap_v[];
extern "C" const size_t animatedmeshcubeshadowmap_vLength;

extern "C" const char meshshadowmap_p[];
extern "C" const size_t meshshadowmap_pLength;
extern "C" const char meshcubeshadowmap_p[];
extern "C" const size_t meshcubeshadowmap_pLength;

extern "C" const char ui_v[];
extern "C" const size_t ui_vLength;
extern "C" const char widget_p[];
extern "C" const size_t widget_pLength;
extern "C" const char font_p[];
extern "C" const size_t font_pLength;

namespace garlic::clove {
    ForwardRenderer3D::ForwardRenderer3D(GhaDevice *graphicsDevice, std::unique_ptr<RenderTarget> renderTarget)
        : graphicsDevice{ graphicsDevice }
        , renderTarget{ std::move(renderTarget) } {
        shaderIncludes["Constants.glsl"] = { constants, constantsLength };

        renderTargetPropertyChangedBeginHandle = this->renderTarget->onPropertiesChangedBegin.bind(&ForwardRenderer3D::cleanupRenderTargetResources, this);
        renderTargetPropertyChangedEndHandle   = this->renderTarget->onPropertiesChangedEnd.bind(&ForwardRenderer3D::createRenderTargetResources, this);

        graphicsFactory = graphicsDevice->getGraphicsFactory();

        //Object initialisation
        graphicsQueue = *graphicsFactory->createGraphicsQueue({ QueueFlags::ReuseBuffers });

        descriptorSetLayouts = createDescriptorSetLayouts(*graphicsFactory);

        createRenderpass();
        createShadowMapRenderpass();

        //Creating here as it's after the render pass is initialised
        colourPass = std::make_unique<ForwardColourPass>(graphicsFactory.get(), renderPass);

        createUiPipeline();
        createShadowMapPipeline();
        createCubeShadowMapPipeline();

        createRenderTargetResources();

        //Create semaphores for frame synchronisation
        for(auto &shadowFinishedSemaphore : shadowFinishedSemaphores) {
            shadowFinishedSemaphore = graphicsFactory->createSemaphore().getValue();
        }
        for(auto &cubeShadowFinishedSemaphore : cubeShadowFinishedSemaphores) {
            cubeShadowFinishedSemaphore = graphicsFactory->createSemaphore().getValue();
        }

        textureSampler = *graphicsFactory->createSampler(GhaSampler::Descriptor{
            .minFilter        = GhaSampler::Filter::Linear,
            .magFilter        = GhaSampler::Filter::Linear,
            .addressModeU     = GhaSampler::AddressMode::Repeat,
            .addressModeV     = GhaSampler::AddressMode::Repeat,
            .addressModeW     = GhaSampler::AddressMode::Repeat,
            .enableAnisotropy = true,
            .maxAnisotropy    = 16.0f,
        });

        uiSampler = *graphicsFactory->createSampler(GhaSampler::Descriptor{
            .minFilter        = GhaSampler::Filter::Nearest,
            .magFilter        = GhaSampler::Filter::Nearest,
            .addressModeU     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeV     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeW     = GhaSampler::AddressMode::ClampToBorder,
            .enableAnisotropy = false,
        });

        shadowSampler = *graphicsFactory->createSampler(GhaSampler::Descriptor{
            .minFilter        = GhaSampler::Filter::Linear,
            .magFilter        = GhaSampler::Filter::Linear,
            .addressModeU     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeV     = GhaSampler::AddressMode::ClampToBorder,
            .addressModeW     = GhaSampler::AddressMode::ClampToBorder,
            .enableAnisotropy = false,
        });

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

        uiMesh = std::make_unique<Mesh>(std::move(uiVertices), std::move(uiIndices));
    }

    //ForwardRenderer3D::ForwardRenderer3D(ForwardRenderer3D&& other) noexcept = default;

    ForwardRenderer3D &ForwardRenderer3D::operator=(ForwardRenderer3D &&other) noexcept = default;

    ForwardRenderer3D::~ForwardRenderer3D() {
        //Wait for an idle device before shutting down so resources aren't freed while in use
        graphicsDevice->waitForIdleDevice();

        //Reset these manually as they would fail after the device has been destroyed (how to solve this?)
        textureSampler.reset();
        for(auto &imageData : inFlightImageData) {
            imageData.frameDataBuffer.reset();
            graphicsQueue->freeCommandBuffer(*imageData.commandBuffer);
        }
    }

    void ForwardRenderer3D::begin() {
        currentFrameData.meshes.clear();
        currentFrameData.widgets.clear();
        currentFrameData.text.clear();

        currentFrameData.bufferData.numLights.numDirectional = 0;
        currentFrameData.bufferData.numLights.numPoint       = 0;
    }

    void ForwardRenderer3D::submitCamera(Camera const &camera, vec3f position) {
        currentFrameData.bufferData.viewData.view       = camera.getView();
        currentFrameData.bufferData.viewData.projection = camera.getProjection();

        currentFrameData.bufferData.viewPosition = position;
    }

    void ForwardRenderer3D::submitLight(DirectionalLight const &light) {
        uint32_t const lightIndex = currentFrameData.bufferData.numLights.numDirectional++;

        currentFrameData.bufferData.lights.directionalLights[lightIndex]    = light.data;
        currentFrameData.bufferData.directionalShadowTransforms[lightIndex] = light.shadowTransform;
    }

    void ForwardRenderer3D::submitLight(PointLight const &light) {
        uint32_t const lightIndex = currentFrameData.bufferData.numLights.numPoint++;

        currentFrameData.bufferData.lights.pointLights[lightIndex] = light.data;
        currentFrameData.pointShadowTransforms[lightIndex]         = light.shadowTransforms;
    }

    void ForwardRenderer3D::submitWidget(std::shared_ptr<GhaImageView> const widget, mat4f const modelProjection) {
        currentFrameData.widgets.push_back({ std::move(widget), std::move(modelProjection) });
    }

    void ForwardRenderer3D::submitText(std::shared_ptr<GhaImageView> const text, mat4f const modelProjection) {
        currentFrameData.text.push_back({ std::move(text), std::move(modelProjection) });
    }

    void ForwardRenderer3D::end() {
        //Aquire the next available image from the render target
        Expected<uint32_t, std::string> const result{ renderTarget->aquireNextImage(currentFrame) };
        if(!result.hasValue()) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Debug, result.getError());
            return;
        }

        size_t const imageIndex{ result.getValue() };

        ImageData &currentImageData = inFlightImageData[imageIndex];

        //Rendering constants / globals
        RenderArea renderArea{
            .origin = { 0, 0 },
            .size   = renderTarget->getSize(),
        };
        RenderArea shadowArea{
            .origin = { 0, 0 },
            .size   = { shadowMapSize, shadowMapSize }
        };

        vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };
        DepthStencilValue depthStencilClearValue{ 1.0f, 0 };

        std::array<ClearValue, 2> outputClearValues{
            clearColour,
            depthStencilClearValue
        };
        std::array<ClearValue, 1> shadowMapClearValues{
            depthStencilClearValue
        };

        size_t const meshCount{ currentFrameData.meshes.size() };

        //We can just write the struct straight in as all the mappings are based off of it's layout
        currentImageData.frameDataBuffer->write(&currentFrameData, 0, sizeof(currentFrameData));

        //Map any non-UBO pieces of data (such as textures / shadow maps)
        currentImageData.lightingDescriptorSet->map(currentImageData.shadowMapViews, *shadowSampler, GhaImage::Layout::ShaderReadOnlyOptimal, 3);
        currentImageData.lightingDescriptorSet->map(currentImageData.cubeShadowMapViews, *shadowSampler, GhaImage::Layout::ShaderReadOnlyOptimal, 4);

        //Allocate a descriptor set for each mesh to be drawn
        std::vector<std::shared_ptr<GhaDescriptorSet>> meshSets;
        if(meshCount > 0) {
            if(currentImageData.meshDescriptorPool == nullptr || currentImageData.meshDescriptorPool->getDescriptor().maxSets < meshCount) {
                auto meshSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::Mesh]);
                for(auto &[key, val] : meshSetBindingCount) {
                    val *= meshCount;
                }
                currentImageData.meshDescriptorPool = createDescriptorPool(meshSetBindingCount, meshCount);
            }

            currentImageData.meshDescriptorPool->reset();
            std::vector<std::shared_ptr<GhaDescriptorSetLayout>> layouts(meshCount, descriptorSetLayouts[DescriptorSetSlots::Mesh]);
            meshSets = currentImageData.meshDescriptorPool->allocateDescriptorSets(layouts);
        }

        struct MeshUBOLayout {
            alignas(256) ModelData model;
            alignas(256) vec4f colour;
            alignas(256) std::array<mat4f, MAX_JOINTS> matrixPallet;
        };

        auto const writeObjectBuffer = [&graphicsFactory = graphicsFactory](std::unique_ptr<GhaBuffer> &buffer, MeshUBOLayout const &layout) {
            if(buffer == nullptr) {
                buffer = *graphicsFactory->createBuffer(GhaBuffer::Descriptor{
                    .size        = sizeof(layout),
                    .usageFlags  = GhaBuffer::UsageMode::UniformBuffer,
                    .sharingMode = SharingMode::Exclusive,
                    .memoryType  = MemoryType::SystemMemory,
                });
            }
            buffer->write(&layout, 0, sizeof(layout));
        };

        //Populate and map the UBOs for each mesh
        if(std::size(currentImageData.objectBuffers) < meshCount) {
            currentImageData.objectBuffers.resize(meshCount);
        }
        for(size_t index = 0; auto &meshInfo : currentFrameData.meshes) {
            MeshUBOLayout layout{
                .model{
                    meshInfo.transform,
                    inverse(transpose(meshInfo.transform)),
                },
                .colour       = meshInfo.material->colour,
                .matrixPallet = meshInfo.matrixPalet,
            };

            writeObjectBuffer(currentImageData.objectBuffers[index], layout);

            std::shared_ptr<GhaDescriptorSet> &meshDescriptorSet = meshSets[index];
            meshDescriptorSet->map(*meshInfo.material->diffuseView, *textureSampler, GhaImage::Layout::ShaderReadOnlyOptimal, 0);
            meshDescriptorSet->map(*meshInfo.material->specularView, *textureSampler, GhaImage::Layout::ShaderReadOnlyOptimal, 1);
            meshDescriptorSet->map(*currentImageData.objectBuffers[index], offsetof(MeshUBOLayout, model), sizeof(ModelData), 2);
            meshDescriptorSet->map(*currentImageData.objectBuffers[index], offsetof(MeshUBOLayout, matrixPallet), sizeof(mat4f) * MAX_JOINTS, 3);
            meshDescriptorSet->map(*currentImageData.objectBuffers[index], offsetof(MeshUBOLayout, colour), sizeof(vec4f), 4);

            ++index;
        }

        //Lambda used to draw a mesh
        auto const drawMesh = [](GhaGraphicsCommandBuffer &commandBuffer, Mesh const &mesh) {
            commandBuffer.bindVertexBuffer(*mesh.getGhaBuffer(), mesh.getVertexOffset());
            commandBuffer.bindIndexBuffer(*mesh.getGhaBuffer(), mesh.getIndexOffset(), IndexType::Uint16);

            commandBuffer.drawIndexed(mesh.getIndexCount());
        };

        //DIRECTIONAL LIGHT SHADOWS
        currentImageData.shadowMapCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        for(size_t i = 0; i < MAX_LIGHTS; ++i) {
            //Make sure to begin the render pass on the images we don't draw to so their layout is transitioned properly
            currentImageData.shadowMapCommandBuffer->beginRenderPass(*shadowMapRenderPass, *currentImageData.shadowMapFrameBuffers[i], shadowArea, shadowMapClearValues);
            if(i < currentFrameData.bufferData.numLights.numDirectional) {
                mat4f const &pushConstantData{ currentFrameData.bufferData.directionalShadowTransforms[i] };
                size_t const pushConstantSize = sizeof(pushConstantData);

                auto const generateShadowMap = [&](Mesh const &mesh, size_t const index) {
                    currentImageData.shadowMapCommandBuffer->pushConstant(GhaShader::Stage::Vertex, 0, pushConstantSize, &pushConstantData);
                    currentImageData.shadowMapCommandBuffer->bindDescriptorSet(*meshSets[index], static_cast<uint32_t>(DescriptorSetSlots::Mesh));

                    drawMesh(*currentImageData.shadowMapCommandBuffer, mesh);
                };

                //Static
                //currentImageData.shadowMapCommandBuffer->bindPipelineObject(*staticMeshShadowMapPipelineObject);
                //currentFrameData.forEachStaticMesh(generateShadowMap);

                //Animated
                currentImageData.shadowMapCommandBuffer->bindPipelineObject(*animatedMeshShadowMapPipelineObject);
                currentFrameData.forEachMesh(generateShadowMap);
            }
            currentImageData.shadowMapCommandBuffer->endRenderPass();
        }
        currentImageData.shadowMapCommandBuffer->endRecording();

        //Submit the command buffer for the directional shadow map
        GraphicsSubmitInfo shadowSubmitInfo{
            .commandBuffers   = { currentImageData.shadowMapCommandBuffer },
            .signalSemaphores = { shadowFinishedSemaphores[currentFrame] },
        };
        graphicsQueue->submit({ std::move(shadowSubmitInfo) }, nullptr);

        //POINT LIGHT SHADOWS
        currentImageData.cubeShadowMapCommandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        for(size_t i = 0; i < MAX_LIGHTS; ++i) {
            for(size_t j = 0; j < 6; ++j) {
                //Make sure to begin the render pass on the images we don't draw to so their layout is transitioned properly
                currentImageData.cubeShadowMapCommandBuffer->beginRenderPass(*shadowMapRenderPass, *currentImageData.cubeShadowMapFrameBuffers[i][j], shadowArea, shadowMapClearValues);
                if(i < currentFrameData.bufferData.numLights.numPoint) {
                    mat4f const &vertPushConstantData{ currentFrameData.pointShadowTransforms[i][j] };
                    size_t const vertPushConstantSize = sizeof(vertPushConstantData);

                    struct {
                        vec3f pos{};
                        float farPlane{};
                    } const pixelPushConstantData{
                        .pos      = currentFrameData.bufferData.lights.pointLights[i].position,
                        .farPlane = currentFrameData.bufferData.lights.pointLights[i].farPlane,
                    };
                    size_t const pixelPushConstantOffset = vertPushConstantSize;
                    size_t const pixelPushConstantSize   = sizeof(pixelPushConstantData);

                    auto const generateCubeShadowMap = [&](Mesh const &mesh, size_t const index) {
                        currentImageData.cubeShadowMapCommandBuffer->pushConstant(GhaShader::Stage::Vertex, 0, vertPushConstantSize, &vertPushConstantData);
                        currentImageData.cubeShadowMapCommandBuffer->pushConstant(GhaShader::Stage::Pixel, pixelPushConstantOffset, pixelPushConstantSize, &pixelPushConstantData);
                        currentImageData.cubeShadowMapCommandBuffer->bindDescriptorSet(*meshSets[index], static_cast<uint32_t>(DescriptorSetSlots::Mesh));

                        drawMesh(*currentImageData.cubeShadowMapCommandBuffer, mesh);
                    };

                    //Static
                    //currentImageData.cubeShadowMapCommandBuffer->bindPipelineObject(*staticMeshCubeShadowMapPipelineObject);
                    //currentFrameData.forEachStaticMesh(generateCubeShadowMap);

                    //Animated
                    currentImageData.cubeShadowMapCommandBuffer->bindPipelineObject(*animatedMeshCubeShadowMapPipelineObject);
                    currentFrameData.forEachMesh(generateCubeShadowMap);
                }
                currentImageData.cubeShadowMapCommandBuffer->endRenderPass();
            }
        }
        currentImageData.cubeShadowMapCommandBuffer->endRecording();

        //Submit the command buffer for the point shadow map
        GraphicsSubmitInfo cubeShadowSubmitInfo{
            .commandBuffers   = { currentImageData.cubeShadowMapCommandBuffer },
            .signalSemaphores = { cubeShadowFinishedSemaphores[currentFrame] },
        };
        graphicsQueue->submit({ std::move(cubeShadowSubmitInfo) }, nullptr);

        //Allocate a descriptor set for each ui element to be drawn
        size_t const widgetCount{ std::size(currentFrameData.widgets) };
        size_t const textCount{ std::size(currentFrameData.text) };
        size_t const uiElementCount{ widgetCount + textCount };

        std::vector<std::shared_ptr<GhaDescriptorSet>> uiSets;
        if(uiElementCount > 0) {
            if(currentImageData.uiDescriptorPool == nullptr || currentImageData.uiDescriptorPool->getDescriptor().maxSets < uiElementCount) {
                auto uiSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::UI]);
                for(auto &[key, val] : uiSetBindingCount) {
                    val *= uiElementCount;
                }
                currentImageData.uiDescriptorPool = createDescriptorPool(uiSetBindingCount, uiElementCount);
            }

            currentImageData.uiDescriptorPool->reset();
            std::vector<std::shared_ptr<GhaDescriptorSetLayout>> uiLayouts(uiElementCount, descriptorSetLayouts[DescriptorSetSlots::UI]);
            uiSets = currentImageData.uiDescriptorPool->allocateDescriptorSets(uiLayouts);
        }

        //FINAL COLOUR
        {
            currentImageData.commandBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
            currentImageData.commandBuffer->beginRenderPass(*renderPass, *frameBuffers[imageIndex], renderArea, outputClearValues);

            currentImageData.commandBuffer->setViewport(vec2i{ 0 }, renderTarget->getSize());
            currentImageData.commandBuffer->setScissor(vec2i{ 0 }, renderTarget->getSize());

            //TEMP: Manually creating the geometry frame data here
            GeometryPass::FrameData data{
                .meshDescriptorSets    = meshSets,
                .viewDescriptorSet     = currentImageData.viewDescriptorSet,
                .lightingDescriptorSet = currentImageData.lightingDescriptorSet,
            };

            colourPass->flushJobs(*currentImageData.commandBuffer, data);

            //Widgets
            currentImageData.commandBuffer->bindPipelineObject(*widgetPipelineObject);
            for(size_t index{ 0 }; auto &&[texture, modelProj] : currentFrameData.widgets) {
                vec4f constexpr colour{ 1.0f };//Temp colour

                uiSets[index]->map(*texture, *uiSampler, GhaImage::Layout::ShaderReadOnlyOptimal, 0);

                currentImageData.commandBuffer->bindDescriptorSet(*uiSets[index], 0);
                currentImageData.commandBuffer->pushConstant(GhaShader::Stage::Vertex, 0, sizeof(modelProj), &modelProj);
                currentImageData.commandBuffer->pushConstant(GhaShader::Stage::Pixel, sizeof(modelProj), sizeof(colour), &colour);

                drawMesh(*currentImageData.commandBuffer, *uiMesh);

                ++index;
            }

            //Text
            currentImageData.commandBuffer->bindPipelineObject(*textPipelineObject);
            for(size_t index{ widgetCount }; auto &&[texture, modelProj] : currentFrameData.text) {
                vec4f constexpr colour{ 1.0f };//Temp colour

                uiSets[index]->map(*texture, *uiSampler, GhaImage::Layout::ShaderReadOnlyOptimal, 0);

                currentImageData.commandBuffer->bindDescriptorSet(*uiSets[index], 0);
                currentImageData.commandBuffer->pushConstant(GhaShader::Stage::Vertex, 0, sizeof(modelProj), &modelProj);
                currentImageData.commandBuffer->pushConstant(GhaShader::Stage::Pixel, sizeof(modelProj), sizeof(colour), &colour);

                drawMesh(*currentImageData.commandBuffer, *uiMesh);

                ++index;
            }

            currentImageData.commandBuffer->endRenderPass();
            currentImageData.commandBuffer->endRecording();
        }

        //Submit the colour output to the render target
        GraphicsSubmitInfo submitInfo{
            .waitSemaphores = {
                { shadowFinishedSemaphores[currentFrame], GhaPipelineObject::Stage::PixelShader },
                { cubeShadowFinishedSemaphores[currentFrame], GhaPipelineObject::Stage::PixelShader },
            },
            .commandBuffers = { currentImageData.commandBuffer },
        };
        renderTarget->submit(imageIndex, currentFrame, std::move(submitInfo));

        currentFrame = (currentFrame + 1) % maxFramesInFlight;
    }

    void ForwardRenderer3D::cleanupRenderTargetResources() {
        graphicsDevice->waitForIdleDevice();

        frameBuffers.clear();

        for(auto &imageData : inFlightImageData) {
            graphicsQueue->freeCommandBuffer(*imageData.commandBuffer);
        }
    }

    void ForwardRenderer3D::createRenderTargetResources() {
        createDepthBuffer();
        createRenderTargetFrameBuffers();

        size_t const imageCount{ std::size(frameBuffers) };

        inFlightImageData.resize(imageCount);

        //Allocate frame scope descriptor pools
        auto viewSetBindingCount{ countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::View]) };
        auto lightingSetBindingCount{ countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::Lighting]) };

        uint32_t constexpr totalSets{ 2 };//Only 2 sets will be allocated from these pools (view + lighting)
        auto bindingCounts = viewSetBindingCount;
        for(auto &&[type, count] : lightingSetBindingCount) {
            bindingCounts[type] += count;
        }

        for(auto &imageData : inFlightImageData) {
            //Create command buffers
            imageData.commandBuffer              = graphicsQueue->allocateCommandBuffer();
            imageData.shadowMapCommandBuffer     = graphicsQueue->allocateCommandBuffer();
            imageData.cubeShadowMapCommandBuffer = graphicsQueue->allocateCommandBuffer();

            //Create uniform buffers
            imageData.frameDataBuffer = *graphicsFactory->createBuffer(GhaBuffer::Descriptor{
                .size        = sizeof(FrameData),
                .usageFlags  = GhaBuffer::UsageMode::UniformBuffer,
                .sharingMode = SharingMode::Exclusive,
                .memoryType  = MemoryType::SystemMemory,
            });

            //Allocate frame scope descriptor Sets
            imageData.frameDescriptorPool = createDescriptorPool(bindingCounts, totalSets);

            imageData.viewDescriptorSet     = imageData.frameDescriptorPool->allocateDescriptorSets(descriptorSetLayouts[DescriptorSetSlots::View]);
            imageData.lightingDescriptorSet = imageData.frameDescriptorPool->allocateDescriptorSets(descriptorSetLayouts[DescriptorSetSlots::Lighting]);

            //As we only have one UBO per frame for every GhaDescriptorSet we can map the buffer into them straight away
            imageData.viewDescriptorSet->map(*imageData.frameDataBuffer, offsetof(FrameData::BufferData, viewData), sizeof(currentFrameData.bufferData.viewData), 0);
            imageData.viewDescriptorSet->map(*imageData.frameDataBuffer, offsetof(FrameData::BufferData, viewPosition), sizeof(currentFrameData.bufferData.viewPosition), 1);

            imageData.lightingDescriptorSet->map(*imageData.frameDataBuffer, offsetof(FrameData::BufferData, lights), sizeof(currentFrameData.bufferData.lights), 0);
            imageData.lightingDescriptorSet->map(*imageData.frameDataBuffer, offsetof(FrameData::BufferData, numLights), sizeof(currentFrameData.bufferData.numLights), 1);
            imageData.lightingDescriptorSet->map(*imageData.frameDataBuffer, offsetof(FrameData::BufferData, directionalShadowTransforms), sizeof(currentFrameData.bufferData.directionalShadowTransforms), 2);

            //Create the shadow maps for each frame
            for(size_t i = 0; i < MAX_LIGHTS; ++i) {
                //Directional
                imageData.shadowMaps[i]     = *graphicsFactory->createImage(GhaImage::Descriptor{
                    .type        = GhaImage::Type::_2D,
                    .usageFlags  = GhaImage::UsageMode::Sampled | GhaImage::UsageMode::DepthStencilAttachment,
                    .dimensions  = { shadowMapSize, shadowMapSize },
                    .format      = GhaImage::Format::D32_SFLOAT,
                    .sharingMode = SharingMode::Exclusive,
                });
                imageData.shadowMapViews[i] = imageData.shadowMaps[i]->createView(GhaImageView::Descriptor{
                    .type       = GhaImageView::Type::_2D,
                    .layer      = 0,
                    .layerCount = 1,
                });

                imageData.shadowMapFrameBuffers[i] = *graphicsFactory->createFramebuffer(GhaFramebuffer::Descriptor{
                    .renderPass  = shadowMapRenderPass,
                    .attachments = { imageData.shadowMapViews[i] },
                    .width       = shadowMapSize,
                    .height      = shadowMapSize,
                });

                //Point
                imageData.cubeShadowMaps[i]     = *graphicsFactory->createImage(GhaImage::Descriptor{
                    .type        = GhaImage::Type::Cube,
                    .usageFlags  = GhaImage::UsageMode::Sampled | GhaImage::UsageMode::DepthStencilAttachment,
                    .dimensions  = { shadowMapSize, shadowMapSize },
                    .format      = GhaImage::Format::D32_SFLOAT,
                    .sharingMode = SharingMode::Exclusive,
                });
                imageData.cubeShadowMapViews[i] = imageData.cubeShadowMaps[i]->createView(GhaImageView::Descriptor{
                    .type       = GhaImageView::Type::Cube,
                    .layer      = 0,
                    .layerCount = 6,
                });

                for(size_t j = 0; j < 6; ++j) {
                    imageData.cubeShadowMapFaceViews[i][j] = imageData.cubeShadowMaps[i]->createView(GhaImageView::Descriptor{
                        .type       = GhaImageView::Type::_2D,
                        .layer      = static_cast<uint32_t>(j),
                        .layerCount = 1,
                    });

                    imageData.cubeShadowMapFrameBuffers[i][j] = *graphicsFactory->createFramebuffer(GhaFramebuffer::Descriptor{
                        .renderPass  = shadowMapRenderPass,
                        .attachments = { imageData.cubeShadowMapFaceViews[i][j] },
                        .width       = shadowMapSize,
                        .height      = shadowMapSize,
                    });
                }
            }
        }
    }

    void ForwardRenderer3D::createRenderpass() {
        //Define what attachments we have
        AttachmentDescriptor colourAttachment{
            .format         = renderTarget->getImageFormat(),
            .loadOperation  = LoadOperation::Clear,
            .storeOperation = StoreOperation::Store,
            .initialLayout  = GhaImage::Layout::Undefined,
            .finalLayout    = GhaImage::Layout::Present,
        };

        AttachmentDescriptor depthAttachment{
            .format         = GhaImage::Format::D32_SFLOAT,
            .loadOperation  = LoadOperation::Clear,
            .storeOperation = StoreOperation::DontCare,
            .initialLayout  = GhaImage::Layout::Undefined,
            .finalLayout    = GhaImage::Layout::DepthStencilAttachmentOptimal,
        };

        //Define attachment references so the subpass knows which slot each attachment will be in
        AttachmentReference colourReference{
            .attachmentIndex = 0,
            .layout          = GhaImage::Layout::ColourAttachmentOptimal,
        };

        AttachmentReference depthReference{
            .attachmentIndex = 1,
            .layout          = GhaImage::Layout::DepthStencilAttachmentOptimal,
        };

        SubpassDescriptor subpass{
            .colourAttachments = { colourReference },
            .depthAttachment   = depthReference,
        };

        //Wait on the implicit subpass at the start. This is so the subpass can transition the layout at the right time
        SubpassDependency dependency{
            .sourceSubpass      = SUBPASS_EXTERNAL,
            .destinationSubpass = 0,
            .sourceStage        = GhaPipelineObject::Stage::ColourAttachmentOutput,
            .destinationStage   = GhaPipelineObject::Stage::ColourAttachmentOutput,
            .currentAccess      = AccessFlags::None,
            .newAccess          = AccessFlags::ColourAttachmentWrite,
        };

        //Create render pass
        GhaRenderPass::Descriptor renderPassDescriptor{
            .attachments  = { std::move(colourAttachment), std::move(depthAttachment) },
            .subpasses    = { std::move(subpass) },
            .dependencies = { std::move(dependency) },
        };

        renderPass = *graphicsFactory->createRenderPass(std::move(renderPassDescriptor));
    }

    void ForwardRenderer3D::createShadowMapRenderpass() {
        AttachmentDescriptor depthAttachment{
            .format         = GhaImage::Format::D32_SFLOAT,
            .loadOperation  = LoadOperation::Clear,
            .storeOperation = StoreOperation::Store,
            .initialLayout  = GhaImage::Layout::Undefined,
            .finalLayout    = GhaImage::Layout::ShaderReadOnlyOptimal,
        };

        AttachmentReference depthReference{
            .attachmentIndex = 0,
            .layout          = GhaImage::Layout::DepthStencilAttachmentOptimal,
        };

        SubpassDescriptor subpass{
            .colourAttachments = {},
            .depthAttachment   = depthReference,
        };

        GhaRenderPass::Descriptor renderPassDescriptor{
            .attachments  = { std::move(depthAttachment) },
            .subpasses    = { std::move(subpass) },
            .dependencies = {},
        };

        shadowMapRenderPass = *graphicsFactory->createRenderPass(std::move(renderPassDescriptor));
    }

    void ForwardRenderer3D::createDepthBuffer() {
        depthImage     = *graphicsFactory->createImage(GhaImage::Descriptor{
            .type        = GhaImage::Type::_2D,
            .usageFlags  = GhaImage::UsageMode::DepthStencilAttachment,
            .dimensions  = renderTarget->getSize(),
            .format      = GhaImage::Format::D32_SFLOAT,
            .sharingMode = SharingMode::Exclusive,
        });
        depthImageView = depthImage->createView(GhaImageView::Descriptor{
            .type       = GhaImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }

    void ForwardRenderer3D::createUiPipeline() {
        size_t constexpr totalAttributes{ 2 };
        std::vector<VertexAttributeDescriptor> vertexAttributes{};
        vertexAttributes.reserve(totalAttributes);

        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 0,
            .format   = VertexAttributeFormat::R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, position),
        });
        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 1,
            .format   = VertexAttributeFormat::R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, texCoord),
        });

        AreaDescriptor viewScissorArea{
            .state = ElementState::Dynamic,
        };

        DepthStateDescriptor depthState{
            .depthTest  = false,
            .depthWrite = false,
        };

        PushConstantDescriptor vertexPushConstant{
            .stage  = GhaShader::Stage::Vertex,
            .offset = 0,
            .size   = sizeof(mat4f),
        };
        PushConstantDescriptor pixelPushConstant{
            .stage  = GhaShader::Stage::Pixel,
            .offset = vertexPushConstant.size,
            .size   = sizeof(vec4f),
        };

        GhaPipelineObject::Descriptor pipelineDescriptor{
            .vertexShader         = *graphicsFactory->createShaderFromSource({ ui_v, ui_vLength }, shaderIncludes, "UI (vertex)", GhaShader::Stage::Vertex),
            .fragmentShader       = *graphicsFactory->createShaderFromSource({ widget_p, widget_pLength }, shaderIncludes, "Widget (pixel)", GhaShader::Stage::Pixel),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = vertexAttributes,
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .depthState           = depthState,
            .renderPass           = renderPass,
            .descriptorSetLayouts = { descriptorSetLayouts[DescriptorSetSlots::UI] },
            .pushConstants        = { std::move(vertexPushConstant), std::move(pixelPushConstant) },
        };

        widgetPipelineObject = *graphicsFactory->createPipelineObject(pipelineDescriptor);

        pipelineDescriptor.fragmentShader = *graphicsFactory->createShaderFromSource({ font_p, font_pLength }, shaderIncludes, "Font (pixel)", GhaShader::Stage::Pixel);

        textPipelineObject = *graphicsFactory->createPipelineObject(std::move(pipelineDescriptor));
    }

    void ForwardRenderer3D::createShadowMapPipeline() {
        size_t constexpr totalAttributes{ 3 };
        std::vector<VertexAttributeDescriptor> vertexAttributes{};
        vertexAttributes.reserve(totalAttributes);

        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 0,
            .format   = VertexAttributeFormat::R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, position),
        });

        PushConstantDescriptor pushConstant{
            .stage = GhaShader::Stage::Vertex,
            .size  = sizeof(mat4f),
        };

        AreaDescriptor viewScissorArea{
            .state    = ElementState::Static,
            .position = { 0.0f, 0.0f },
            .size     = { shadowMapSize, shadowMapSize }
        };

        GhaPipelineObject::Descriptor pipelineDescriptor{
            .vertexShader         = *graphicsFactory->createShaderFromSource({ staticmeshshadowmap_v, staticmeshshadowmap_vLength }, shaderIncludes, "Shadow Map - Static Mesh (vertex)", GhaShader::Stage::Vertex),
            .fragmentShader       = *graphicsFactory->createShaderFromSource({ meshshadowmap_p, meshshadowmap_pLength }, shaderIncludes, "Shadow Map (pixel)", GhaShader::Stage::Pixel),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = vertexAttributes,
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .enableBlending       = false,
            .renderPass           = shadowMapRenderPass,
            .descriptorSetLayouts = { descriptorSetLayouts[DescriptorSetSlots::Mesh] },
            .pushConstants        = { pushConstant },
        };

        //staticMeshShadowMapPipelineObject = *graphicsFactory->createPipelineObject(pipelineDescriptor);

        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 4,
            .format   = VertexAttributeFormat::R32G32B32A32_SINT,
            .offset   = offsetof(Vertex, jointIds),
        });
        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 5,
            .format   = VertexAttributeFormat::R32G32B32A32_SFLOAT,
            .offset   = offsetof(Vertex, weights),
        });

        pipelineDescriptor.vertexShader         = *graphicsFactory->createShaderFromSource({ animatedmeshshadowmap_v, animatedmeshshadowmap_vLength }, shaderIncludes, "Shadow Map - Animated Mesh (vertex)", GhaShader::Stage::Vertex);
        pipelineDescriptor.vertexAttributes     = std::move(vertexAttributes);
        pipelineDescriptor.descriptorSetLayouts = { descriptorSetLayouts[DescriptorSetSlots::Mesh] };

        animatedMeshShadowMapPipelineObject = *graphicsFactory->createPipelineObject(pipelineDescriptor);
    }

    void ForwardRenderer3D::createCubeShadowMapPipeline() {
        size_t constexpr totalAttributes{ 3 };
        std::vector<VertexAttributeDescriptor> vertexAttributes{};
        vertexAttributes.reserve(totalAttributes);

        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 0,
            .format   = VertexAttributeFormat::R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, position),
        });

        PushConstantDescriptor vertexPushConstant{
            .stage  = GhaShader::Stage::Vertex,
            .offset = 0,
            .size   = sizeof(mat4f),
        };
        PushConstantDescriptor pixelPushConstant{
            .stage  = GhaShader::Stage::Pixel,
            .offset = vertexPushConstant.size,
            .size   = sizeof(vec3f) + sizeof(float),
        };

        AreaDescriptor viewScissorArea{
            .state    = ElementState::Static,
            .position = { 0.0f, 0.0f },
            .size     = { shadowMapSize, shadowMapSize }
        };

        GhaPipelineObject::Descriptor pipelineDescriptor{
            .vertexShader         = *graphicsFactory->createShaderFromSource({ staticmeshcubeshadowmap_v, staticmeshcubeshadowmap_vLength }, shaderIncludes, "Cube Shadow Map - Static Mesh (vertex)", GhaShader::Stage::Vertex),
            .fragmentShader       = *graphicsFactory->createShaderFromSource({ meshcubeshadowmap_p, meshcubeshadowmap_pLength }, shaderIncludes, "Cube Shadow Map (pixel)", GhaShader::Stage::Pixel),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = vertexAttributes,
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .enableBlending       = false,
            .renderPass           = shadowMapRenderPass,
            .descriptorSetLayouts = { descriptorSetLayouts[DescriptorSetSlots::Mesh] },
            .pushConstants        = { vertexPushConstant, pixelPushConstant },
        };

        //staticMeshCubeShadowMapPipelineObject = *graphicsFactory->createPipelineObject(pipelineDescriptor);

        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 4,
            .format   = VertexAttributeFormat::R32G32B32A32_SINT,
            .offset   = offsetof(Vertex, jointIds),
        });
        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 5,
            .format   = VertexAttributeFormat::R32G32B32A32_SFLOAT,
            .offset   = offsetof(Vertex, weights),
        });

        pipelineDescriptor.vertexShader         = *graphicsFactory->createShaderFromSource({ animatedmeshcubeshadowmap_v, animatedmeshcubeshadowmap_vLength }, shaderIncludes, "Cube Shadow Map - Animated Mesh (vertex)", GhaShader::Stage::Vertex);
        pipelineDescriptor.vertexAttributes     = std::move(vertexAttributes);
        pipelineDescriptor.descriptorSetLayouts = { descriptorSetLayouts[DescriptorSetSlots::Mesh] };

        animatedMeshCubeShadowMapPipelineObject = *graphicsFactory->createPipelineObject(pipelineDescriptor);
    }

    void ForwardRenderer3D::createRenderTargetFrameBuffers() {
        for(auto &imageView : renderTarget->getImageViews()) {
            frameBuffers.emplace_back(*graphicsFactory->createFramebuffer(GhaFramebuffer::Descriptor{
                .renderPass  = renderPass,
                .attachments = { imageView, depthImageView },
                .width       = renderTarget->getSize().x,
                .height      = renderTarget->getSize().y,
            }));
        }
    }

    std::shared_ptr<GhaDescriptorPool> ForwardRenderer3D::createDescriptorPool(std::unordered_map<DescriptorType, uint32_t> const &bindingCount, uint32_t const setCount) {
        std::vector<DescriptorInfo> poolTypes;
        for(auto &&[type, count] : bindingCount) {
            poolTypes.emplace_back(DescriptorInfo{
                .type  = type,
                .count = count,
            });
        }

        GhaDescriptorPool::Descriptor poolDescriptor{
            .poolTypes = std::move(poolTypes),
            .flag      = GhaDescriptorPool::Flag::None,
            .maxSets   = setCount,
        };

        return *graphicsFactory->createDescriptorPool(std::move(poolDescriptor));
    }
}
