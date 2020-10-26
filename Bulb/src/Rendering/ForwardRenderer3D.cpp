#include "Bulb/Rendering/ForwardRenderer3D.hpp"

#include "Bulb/Rendering/Camera.hpp"
#include "Bulb/Rendering/Material.hpp"
#include "Bulb/Rendering/Renderables/Mesh.hpp"
#include "Bulb/Rendering/RenderingHelpers.hpp"
#include "Bulb/Rendering/Vertex.hpp"
#include "Bulb/TextureLoader.hpp"

#include <Clove/Graphics/DescriptorSet.hpp>
#include <Clove/Graphics/Graphics.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>
#include <Clove/Platform/Window.hpp>
#include <Root/Log/Log.hpp>

using namespace clv::gfx;

extern "C" const char staticmesh_v[];
extern "C" const size_t staticmesh_vLength;
extern "C" const char staticmeshshadowmap_v[];
extern "C" const size_t staticmeshshadowmap_vLength;
extern "C" const char staticmeshcubeshadowmap_v[];
extern "C" const size_t staticmeshcubeshadowmap_vLength;

extern "C" const char animatedmesh_v[];
extern "C" const size_t animatedmesh_vLength;
extern "C" const char animatedmeshshadowmap_v[];
extern "C" const size_t animatedmeshshadowmap_vLength;
extern "C" const char animatedmeshcubeshadowmap_v[];
extern "C" const size_t animatedmeshcubeshadowmap_vLength;

extern "C" const char mesh_p[];
extern "C" const size_t mesh_pLength;
extern "C" const char meshshadowmap_p[];
extern "C" const size_t meshshadowmap_pLength;
extern "C" const char meshcubeshadowmap_p[];
extern "C" const size_t meshcubeshadowmap_pLength;

namespace blb::rnd {
    ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window &window, API const api) {
        windowResizeHandle = window.onWindowResize.bind(&ForwardRenderer3D::onWindowResize, this);
        windowSize         = window.getSize();

        graphicsDevice  = createGraphicsDevice(api, window.getNativeWindow());
        graphicsFactory = graphicsDevice->getGraphicsFactory();

        //Object initialisation
        graphicsQueue = graphicsFactory->createGraphicsQueue({ QueueFlags::ReuseBuffers });
        presentQueue  = graphicsFactory->createPresentQueue();

        swapchain = graphicsFactory->createSwapChain({ windowSize });

        descriptorSetLayouts = createDescriptorSetLayouts(*graphicsFactory);

        createRenderpass();
        createShadowMapRenderpass();

        createShadowMapPipeline();
        createCubeShadowMapPipeline();

        createDepthBuffer();

        recreateSwapchain();//Also creates the pipeline for the final colour

        //Create semaphores for frame synchronisation
        for(auto &shadowFinishedSemaphore : shadowFinishedSemaphores) {
            shadowFinishedSemaphore = graphicsFactory->createSemaphore();
        }
        for(auto &cubeShadowFinishedSemaphore : cubeShadowFinishedSemaphores) {
            cubeShadowFinishedSemaphore = graphicsFactory->createSemaphore();
        }
        for(auto &renderFinishedSemaphore : renderFinishedSemaphores) {
            renderFinishedSemaphore = graphicsFactory->createSemaphore();
        }
        for(auto &imageAvailableSemaphore : imageAvailableSemaphores) {
            imageAvailableSemaphore = graphicsFactory->createSemaphore();
        }
        for(auto &inFlightFence : inFlightFences) {
            inFlightFence = graphicsFactory->createFence({ true });
        }
        imagesInFlight.resize(swapchain->getImageViews().size());

        sampler = graphicsFactory->createSampler(Sampler::Descriptor{
            .minFilter        = Sampler::Filter::Linear,
            .magFilter        = Sampler::Filter::Linear,
            .addressModeU     = Sampler::AddressMode::Repeat,
            .addressModeV     = Sampler::AddressMode::Repeat,
            .addressModeW     = Sampler::AddressMode::Repeat,
            .enableAnisotropy = true,
            .maxAnisotropy    = 16.0f,
        });
    }

    //ForwardRenderer3D::ForwardRenderer3D(ForwardRenderer3D&& other) noexcept = default;

    ForwardRenderer3D &ForwardRenderer3D::operator=(ForwardRenderer3D &&other) noexcept = default;

    ForwardRenderer3D::~ForwardRenderer3D() {
        //Wait for an idle device before shutting down so resources aren't freed while in use
        graphicsDevice->waitForIdleDevice();

        //Reset these manually as they would fail after the device has been destroyed (how to solve this?)
        sampler.reset();
        for(auto &imageData : inFlightImageData) {
            imageData.uniformBuffer.reset();
            graphicsQueue->freeCommandBuffer(*imageData.commandBuffer);
        }
    }

    void ForwardRenderer3D::begin() {
        currentFrameData.staticMeshes.clear();
        currentFrameData.animatedMeshes.clear();
        currentFrameData.bufferData.numLights.numDirectional = 0;
        currentFrameData.bufferData.numLights.numPoint       = 0;
    }

    void ForwardRenderer3D::submitCamera(Camera const &camera, clv::mth::vec3f position) {
        currentFrameData.bufferData.viewData.view       = camera.getView();
        currentFrameData.bufferData.viewData.projection = camera.getProjection();

        currentFrameData.bufferData.viewPosition = position;
    }

    void ForwardRenderer3D::submitStaticMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform) {
        currentFrameData.staticMeshes.push_back({ std::move(mesh), std::move(transform) });
    }

    void ForwardRenderer3D::submitAnimatedMesh(std::shared_ptr<rnd::Mesh> mesh, clv::mth::mat4f transform, std::array<clv::mth::mat4f, blb::rnd::MAX_JOINTS> matrixPalet) {
        currentFrameData.animatedMeshes.push_back({ std::move(mesh), std::move(transform), std::move(matrixPalet) });
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

    void ForwardRenderer3D::submitWidget(std::shared_ptr<Sprite> const &widget) {
    }

    void ForwardRenderer3D::submitText(std::shared_ptr<Sprite> const &text) {
    }

    void ForwardRenderer3D::end() {
        if(needNewSwapchain) {
            recreateSwapchain();
            return;//return early just in case the window was minimised
        }

        //Wait on the current frame / current images to be available
        inFlightFences[currentFrame]->wait();

        //The index of the image we're working on in the swap chain. Might not be equal to the currentFrame index
        uint32_t imageIndex{ 0 };

        //Aquire the next available image
        Result result = swapchain->aquireNextImage(imageAvailableSemaphores[currentFrame].get(), imageIndex);
        if(result == Result::Error_SwapchainOutOfDate) {
            recreateSwapchain();
            return;//return early just in case the window was minimised
        }

        //Check if we're already using this image, if so wait
        if(imagesInFlight[imageIndex] != nullptr) {
            imagesInFlight[imageIndex]->wait();
        }
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];//Ptr copy here, could be slowing things down

        inFlightFences[currentFrame]->reset();

        ImageData &currentImageData = inFlightImageData[imageIndex];

        //Record our command buffers
        RenderArea renderArea{};
        renderArea.origin = { 0, 0 };
        renderArea.size   = { swapchain->getExtent().x, swapchain->getExtent().y };

        RenderArea shadowArea{
            .origin = { 0, 0 },
            .size   = { shadowMapSize, shadowMapSize }
        };

        clv::mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };
        DepthStencilValue depthStencilClearValue{ 1.0f, 0 };

        std::array<ClearValue, 2> outputClearValues{
            ClearValue{ clearColour, {} },
            ClearValue{ {}, depthStencilClearValue }
        };
        std::array<ClearValue, 1> shadowMapClearValues{
            ClearValue{ {}, depthStencilClearValue }
        };

        //We can just write the struct straight in as all the mappings are based off of it's layout
        currentImageData.uniformBuffer->write(&currentFrameData, 0, sizeof(currentFrameData));

        //Map any non-UBO pieces of data (such as textures / shadow maps)
        currentImageData.lightingDescriptorSet->map(currentImageData.shadowMapViews, *sampler, GraphicsImage::Layout::ShaderReadOnlyOptimal, 3);
        currentImageData.lightingDescriptorSet->map(currentImageData.cubeShadowMapViews, *sampler, GraphicsImage::Layout::ShaderReadOnlyOptimal, 4);

        //Start recording
        currentImageData.commandBuffer->beginRecording(CommandBufferUsage::Default);
        currentImageData.shadowMapCommandBuffer->beginRecording(CommandBufferUsage::Default);
        currentImageData.cubeShadowMapCommandBuffer->beginRecording(CommandBufferUsage::Default);

        //Bind the global descriptor sets
        currentImageData.commandBuffer->bindDescriptorSet(*currentImageData.viewDescriptorSet, *staticMeshPipelineObject, static_cast<uint32_t>(DescriptorSetSlots::View));
        currentImageData.commandBuffer->bindDescriptorSet(*currentImageData.lightingDescriptorSet, *staticMeshPipelineObject, static_cast<uint32_t>(DescriptorSetSlots::Lighting));

        size_t const staticMeshCount   = std::size(currentFrameData.staticMeshes);
        size_t const animatedMeshCount = std::size(currentFrameData.animatedMeshes);

        size_t const meshCount = staticMeshCount + animatedMeshCount;

        //Allocate a descriptor set for each mesh to be drawn
        if(currentImageData.meshDescriptorPool == nullptr || currentImageData.meshDescriptorPool->getDescriptor().maxSets < meshCount) {
            auto meshSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::Mesh]);
            for(auto &[key, val] : meshSetBindingCount) {
                val *= meshCount;
            }
            currentImageData.meshDescriptorPool = createDescriptorPool(meshSetBindingCount, meshCount);
        }

        currentImageData.meshDescriptorPool->reset();
        std::vector<std::shared_ptr<DescriptorSetLayout>> layouts(meshCount, descriptorSetLayouts[DescriptorSetSlots::Mesh]);
        std::vector<std::shared_ptr<DescriptorSet>> meshSets = currentImageData.meshDescriptorPool->allocateDescriptorSets(layouts);

        currentImageData.commandBuffer->beginRenderPass(*renderPass, *swapChainFrameBuffers[imageIndex], renderArea, outputClearValues);

        size_t meshIndex = 0;

        //STATIC MESHES
        currentImageData.commandBuffer->bindPipelineObject(*staticMeshPipelineObject);
        currentImageData.shadowMapCommandBuffer->bindPipelineObject(*staticMeshShadowMapPipelineObject);
        currentImageData.cubeShadowMapCommandBuffer->bindPipelineObject(*staticMeshCubeShadowMapPipelineObject);
        for(auto &&[mesh, transform] : currentFrameData.staticMeshes) {
            std::shared_ptr<DescriptorSet> &meshDescriptorSet = meshSets[meshIndex];

            clv::mth::mat4f modelData[]{ transform, clv::mth::inverse(clv::mth::transpose(transform)) };

            meshDescriptorSet->map(*mesh->getMaterial().diffuseView, *sampler, GraphicsImage::Layout::ShaderReadOnlyOptimal, 0);

            for(size_t i = 0; i < MAX_LIGHTS; ++i) {
                //Make sure to begin the render pass on the images we don't draw to so their layout is transitioned properly
                currentImageData.shadowMapCommandBuffer->beginRenderPass(*shadowMapRenderPass, *currentImageData.shadowMapFrameBuffers[i], shadowArea, shadowMapClearValues);

                if(i < currentFrameData.bufferData.numLights.numDirectional) {
                    clv::mth::mat4f const pushConstantData[]{ transform, currentFrameData.bufferData.directionalShadowTransforms[i] };

                    currentImageData.shadowMapCommandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
                    currentImageData.shadowMapCommandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

                    currentImageData.shadowMapCommandBuffer->pushConstant(*staticMeshShadowMapPipelineObject, Shader::Stage::Vertex, 0, sizeof(pushConstantData), pushConstantData);

                    currentImageData.shadowMapCommandBuffer->drawIndexed(mesh->getIndexCount());
                }

                currentImageData.shadowMapCommandBuffer->endRenderPass();

                for(size_t j = 0; j < 6; ++j) {
                    //Make sure to begin the render pass on the images we don't draw to so their layout is transitioned properly
                    currentImageData.cubeShadowMapCommandBuffer->beginRenderPass(*shadowMapRenderPass, *currentImageData.cubeShadowMapFrameBuffers[i][j], shadowArea, shadowMapClearValues);

                    if(i < currentFrameData.bufferData.numLights.numPoint) {
                        clv::mth::mat4f const vertPushConstantData[]{ transform, currentFrameData.pointShadowTransforms[i][j] };
                        struct {
                            clv::mth::vec3f pos{};
                            float farPlane{};
                        } pixelPushConstantData{
                            .pos      = currentFrameData.bufferData.lights.pointLights[i].position,
                            .farPlane = currentFrameData.bufferData.lights.pointLights[i].farPlane,
                        };

                        currentImageData.cubeShadowMapCommandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
                        currentImageData.cubeShadowMapCommandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

                        currentImageData.cubeShadowMapCommandBuffer->pushConstant(*staticMeshCubeShadowMapPipelineObject, Shader::Stage::Vertex, 0, sizeof(vertPushConstantData), vertPushConstantData);
                        currentImageData.cubeShadowMapCommandBuffer->pushConstant(*staticMeshCubeShadowMapPipelineObject, Shader::Stage::Pixel, sizeof(vertPushConstantData), sizeof(pixelPushConstantData), &pixelPushConstantData);

                        currentImageData.cubeShadowMapCommandBuffer->drawIndexed(mesh->getIndexCount());
                    }

                    currentImageData.cubeShadowMapCommandBuffer->endRenderPass();
                }
            }

            currentImageData.commandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
            currentImageData.commandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

            currentImageData.commandBuffer->pushConstant(*staticMeshPipelineObject, Shader::Stage::Vertex, 0, sizeof(modelData), modelData);
            currentImageData.commandBuffer->bindDescriptorSet(*meshDescriptorSet, *staticMeshPipelineObject, static_cast<uint32_t>(DescriptorSetSlots::Mesh));

            currentImageData.commandBuffer->drawIndexed(mesh->getIndexCount());

            ++meshIndex;
        }

        //ANIMATED MESHES
        if(currentImageData.paletBuffers.capacity() < animatedMeshCount) {
            currentImageData.paletBuffers.resize(animatedMeshCount);
        }
        currentImageData.commandBuffer->bindPipelineObject(*animatedMeshPipelineObject);
        currentImageData.shadowMapCommandBuffer->bindPipelineObject(*animatedMeshShadowMapPipelineObject);
        currentImageData.cubeShadowMapCommandBuffer->bindPipelineObject(*animatedMeshCubeShadowMapPipelineObject);
        for(size_t animatedMeshIndex = 0; auto &&[mesh, transform, matrixPalet] : currentFrameData.animatedMeshes) {
            if(currentImageData.paletBuffers[animatedMeshIndex] == nullptr) {
                currentImageData.paletBuffers[animatedMeshIndex] = graphicsFactory->createBuffer(GraphicsBuffer::Descriptor{
                    .size        = sizeof(clv::mth::mat4f) * MAX_JOINTS,
                    .usageFlags  = GraphicsBuffer::UsageMode::UniformBuffer,
                    .sharingMode = SharingMode::Exclusive,
                    .memoryType  = MemoryType::SystemMemory,
                });
            }

            std::shared_ptr<DescriptorSet> &meshDescriptorSet = meshSets[meshIndex];

            currentImageData.paletBuffers[animatedMeshIndex]->write(std::data(matrixPalet), 0, sizeof(matrixPalet));
            clv::mth::mat4f modelData[]{ transform, clv::mth::inverse(clv::mth::transpose(transform)) };

            meshDescriptorSet->map(*mesh->getMaterial().diffuseView, *sampler, GraphicsImage::Layout::ShaderReadOnlyOptimal, 0);
            meshDescriptorSet->map(*currentImageData.paletBuffers[animatedMeshIndex], 0, sizeof(matrixPalet), 1);

            for(size_t i = 0; i < MAX_LIGHTS; ++i) {
                //Make sure to begin the render pass on the images we don't draw to so their layout is transitioned properly
                currentImageData.shadowMapCommandBuffer->beginRenderPass(*shadowMapRenderPass, *currentImageData.shadowMapFrameBuffers[i], shadowArea, shadowMapClearValues);

                if(i < currentFrameData.bufferData.numLights.numDirectional) {
                    clv::mth::mat4f const pushConstantData[]{ transform, currentFrameData.bufferData.directionalShadowTransforms[i] };

                    currentImageData.shadowMapCommandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
                    currentImageData.shadowMapCommandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

                    currentImageData.shadowMapCommandBuffer->pushConstant(*staticMeshShadowMapPipelineObject, Shader::Stage::Vertex, 0, sizeof(pushConstantData), pushConstantData);
                    currentImageData.shadowMapCommandBuffer->bindDescriptorSet(*meshDescriptorSet, *staticMeshPipelineObject, static_cast<uint32_t>(DescriptorSetSlots::Mesh));

                    currentImageData.shadowMapCommandBuffer->drawIndexed(mesh->getIndexCount());
                }

                currentImageData.shadowMapCommandBuffer->endRenderPass();

                for(size_t j = 0; j < 6; ++j) {
                    //Make sure to begin the render pass on the images we don't draw to so their layout is transitioned properly
                    currentImageData.cubeShadowMapCommandBuffer->beginRenderPass(*shadowMapRenderPass, *currentImageData.cubeShadowMapFrameBuffers[i][j], shadowArea, shadowMapClearValues);

                    if(i < currentFrameData.bufferData.numLights.numPoint) {
                        clv::mth::mat4f const vertPushConstantData[]{ transform, currentFrameData.pointShadowTransforms[i][j] };
                        struct {
                            clv::mth::vec3f pos{};
                            float farPlane{};
                        } pixelPushConstantData{
                            .pos      = currentFrameData.bufferData.lights.pointLights[i].position,
                            .farPlane = currentFrameData.bufferData.lights.pointLights[i].farPlane,
                        };

                        currentImageData.cubeShadowMapCommandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
                        currentImageData.cubeShadowMapCommandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

                        currentImageData.cubeShadowMapCommandBuffer->pushConstant(*staticMeshCubeShadowMapPipelineObject, Shader::Stage::Vertex, 0, sizeof(vertPushConstantData), vertPushConstantData);
                        currentImageData.cubeShadowMapCommandBuffer->pushConstant(*staticMeshCubeShadowMapPipelineObject, Shader::Stage::Pixel, sizeof(vertPushConstantData), sizeof(pixelPushConstantData), &pixelPushConstantData);
                        currentImageData.cubeShadowMapCommandBuffer->bindDescriptorSet(*meshDescriptorSet, *staticMeshPipelineObject, static_cast<uint32_t>(DescriptorSetSlots::Mesh));

                        currentImageData.cubeShadowMapCommandBuffer->drawIndexed(mesh->getIndexCount());
                    }

                    currentImageData.cubeShadowMapCommandBuffer->endRenderPass();
                }
            }

            currentImageData.commandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
            currentImageData.commandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

            currentImageData.commandBuffer->pushConstant(*staticMeshPipelineObject, Shader::Stage::Vertex, 0, sizeof(modelData), modelData);
            currentImageData.commandBuffer->bindDescriptorSet(*meshDescriptorSet, *staticMeshPipelineObject, static_cast<uint32_t>(DescriptorSetSlots::Mesh));

            currentImageData.commandBuffer->drawIndexed(mesh->getIndexCount());

            ++meshIndex;
            ++animatedMeshIndex;
        }

        currentImageData.commandBuffer->endRenderPass();

        currentImageData.commandBuffer->endRecording();
        currentImageData.shadowMapCommandBuffer->endRecording();
        currentImageData.cubeShadowMapCommandBuffer->endRecording();

        //Submit the command buffers
        GraphicsSubmitInfo shadowSubmitInfo{
            .commandBuffers   = { currentImageData.shadowMapCommandBuffer },
            .signalSemaphores = { shadowFinishedSemaphores[currentFrame] },
        };
        GraphicsSubmitInfo cubeShadowSubmitInfo{
            .commandBuffers   = { currentImageData.cubeShadowMapCommandBuffer },
            .signalSemaphores = { cubeShadowFinishedSemaphores[currentFrame] },
        };
        GraphicsSubmitInfo submitInfo{
            .waitSemaphores = {
                {
                    shadowFinishedSemaphores[currentFrame],
                    PipelineObject::Stage::PixelShader,
                },
                {
                    cubeShadowFinishedSemaphores[currentFrame],
                    PipelineObject::Stage::PixelShader,
                },
                {
                    imageAvailableSemaphores[currentFrame],
                    PipelineObject::Stage::ColourAttachmentOutput,
                },
            },
            .commandBuffers   = { currentImageData.commandBuffer },
            .signalSemaphores = { renderFinishedSemaphores[currentFrame] },
        };
        graphicsQueue->submit(shadowSubmitInfo, nullptr);
        graphicsQueue->submit(cubeShadowSubmitInfo, nullptr);
        graphicsQueue->submit(submitInfo, inFlightFences[currentFrame].get());

        //Present current image
        result = presentQueue->present(PresentInfo{
            .waitSemaphores = { renderFinishedSemaphores[currentFrame] },
            .swapChain      = swapchain,
            .imageIndex     = imageIndex,
        });

        if(needNewSwapchain || result == Result::Error_SwapchainOutOfDate || result == Result::Success_SwapchainSuboptimal) {
            recreateSwapchain();
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "Swapchain recreated at end of loop");
        }

        currentFrame = (currentFrame + 1) % maxFramesInFlight;
    }

    std::shared_ptr<GraphicsFactory> const &ForwardRenderer3D::getGraphicsFactory() const {
        return graphicsFactory;
    }

    void ForwardRenderer3D::onWindowResize(clv::mth::vec2ui const &size) {
        windowSize       = size;
        needNewSwapchain = true;
    }

    void ForwardRenderer3D::recreateSwapchain() {
        //Set this to true in case we need to wait for the window to unminimise
        needNewSwapchain = true;

        if(windowSize.x == 0 || windowSize.y == 0) {
            return;
        }

        graphicsDevice->waitForIdleDevice();

        //Explicitly free resources to avoid problems when recreating the swap chain itself
        swapchain.reset();
        staticMeshPipelineObject.reset();
        swapChainFrameBuffers.clear();
        for(auto &imageData : inFlightImageData) {
            graphicsQueue->freeCommandBuffer(*imageData.commandBuffer);
        }

        //Recreate our swap chain
        swapchain = graphicsFactory->createSwapChain({ windowSize });

        createRenderpass();

        createDepthBuffer();

        createPipeline();
        createSwapchainFrameBuffers();

        size_t const imageCount = std::size(swapChainFrameBuffers);

        inFlightImageData.resize(imageCount);

        //Allocate frame scope descriptor pools
        auto viewSetBindingCount     = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::View]);
        auto lightingSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::Lighting]);

        uint32_t constexpr totalSets{ 2 };//Only 2 sets will be allocated from these pools (view + lighting)
        auto bindingCounts = viewSetBindingCount;
        bindingCounts.merge(lightingSetBindingCount);

        for(auto &imageData : inFlightImageData) {
            //Create command buffers
            imageData.commandBuffer              = graphicsQueue->allocateCommandBuffer();
            imageData.shadowMapCommandBuffer     = graphicsQueue->allocateCommandBuffer();
            imageData.cubeShadowMapCommandBuffer = graphicsQueue->allocateCommandBuffer();

            //Create uniform buffers
            imageData.uniformBuffer = graphicsFactory->createBuffer(GraphicsBuffer::Descriptor{
                .size        = sizeof(FrameData),
                .usageFlags  = GraphicsBuffer::UsageMode::UniformBuffer,
                .sharingMode = SharingMode::Exclusive,
                .memoryType  = MemoryType::SystemMemory,
            });

            //Allocate frame scope descriptor Sets
            imageData.frameDescriptorPool = createDescriptorPool(bindingCounts, totalSets);

            imageData.viewDescriptorSet     = imageData.frameDescriptorPool->allocateDescriptorSets(descriptorSetLayouts[DescriptorSetSlots::View]);
            imageData.lightingDescriptorSet = imageData.frameDescriptorPool->allocateDescriptorSets(descriptorSetLayouts[DescriptorSetSlots::Lighting]);

            //As we only have one UBO per frame for every DescriptorSet we can map the buffer into them straight away
            imageData.viewDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData::BufferData, viewData), sizeof(currentFrameData.bufferData.viewData), 0);
            imageData.viewDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData::BufferData, viewPosition), sizeof(currentFrameData.bufferData.viewPosition), 1);

            imageData.lightingDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData::BufferData, lights), sizeof(currentFrameData.bufferData.lights), 0);
            imageData.lightingDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData::BufferData, numLights), sizeof(currentFrameData.bufferData.numLights), 1);
            imageData.lightingDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData::BufferData, directionalShadowTransforms), sizeof(currentFrameData.bufferData.directionalShadowTransforms), 2);

            //Create the shadow maps for each frame
            for(size_t i = 0; i < MAX_LIGHTS; ++i) {
                //Directional
                imageData.shadowMaps[i]     = graphicsFactory->createImage(GraphicsImage::Descriptor{
                    .type        = GraphicsImage::Type::_2D,
                    .usageFlags  = GraphicsImage::UsageMode::Sampled | GraphicsImage::UsageMode::DepthStencilAttachment,
                    .dimensions  = { shadowMapSize, shadowMapSize },
                    .format      = GraphicsImage::Format::D32_SFLOAT,
                    .sharingMode = SharingMode::Exclusive,
                    .memoryType  = MemoryType::VideoMemory,
                });
                imageData.shadowMapViews[i] = imageData.shadowMaps[i]->createView(GraphicsImageView::Descriptor{
                    .type       = GraphicsImageView::Type::_2D,
                    .layer      = 0,
                    .layerCount = 1,
                });

                imageData.shadowMapFrameBuffers[i] = graphicsFactory->createFramebuffer(Framebuffer::Descriptor{
                    .renderPass  = shadowMapRenderPass,
                    .attachments = { imageData.shadowMapViews[i] },
                    .width       = shadowMapSize,
                    .height      = shadowMapSize,
                });

                //Point
                imageData.cubeShadowMaps[i]     = graphicsFactory->createImage(GraphicsImage::Descriptor{
                    .type        = GraphicsImage::Type::Cube,
                    .usageFlags  = GraphicsImage::UsageMode::Sampled | GraphicsImage::UsageMode::DepthStencilAttachment,
                    .dimensions  = { shadowMapSize, shadowMapSize },
                    .format      = GraphicsImage::Format::D32_SFLOAT,
                    .sharingMode = SharingMode::Exclusive,
                    .memoryType  = MemoryType::VideoMemory,
                });
                imageData.cubeShadowMapViews[i] = imageData.cubeShadowMaps[i]->createView(GraphicsImageView::Descriptor{
                    .type       = GraphicsImageView::Type::Cube,
                    .layer      = 0,
                    .layerCount = 6,
                });

                for(size_t j = 0; j < 6; ++j) {
                    imageData.cubeShadowMapFaceViews[i][j] = imageData.cubeShadowMaps[i]->createView(GraphicsImageView::Descriptor{
                        .type       = GraphicsImageView::Type::_2D,
                        .layer      = static_cast<uint32_t>(j),
                        .layerCount = 1,
                    });

                    imageData.cubeShadowMapFrameBuffers[i][j] = graphicsFactory->createFramebuffer(Framebuffer::Descriptor{
                        .renderPass  = shadowMapRenderPass,
                        .attachments = { imageData.cubeShadowMapFaceViews[i][j] },
                        .width       = shadowMapSize,
                        .height      = shadowMapSize,
                    });
                }
            }
        }

        needNewSwapchain = false;
    }

    void ForwardRenderer3D::createRenderpass() {
        //Define what attachments we have
        AttachmentDescriptor colourAttachment{
            .format         = swapchain->getImageFormat(),
            .loadOperation  = LoadOperation::Clear,
            .storeOperation = StoreOperation::Store,
            .initialLayout  = GraphicsImage::Layout::Undefined,
            .finalLayout    = GraphicsImage::Layout::Present,
        };

        AttachmentDescriptor depthAttachment{
            .format         = GraphicsImage::Format::D32_SFLOAT,
            .loadOperation  = LoadOperation::Clear,
            .storeOperation = StoreOperation::DontCare,
            .initialLayout  = GraphicsImage::Layout::Undefined,
            .finalLayout    = GraphicsImage::Layout::DepthStencilAttachmentOptimal,
        };

        //Define attachment references so the subpass knows which slot each attachment will be in
        AttachmentReference colourReference{
            .attachmentIndex = 0,
            .layout          = GraphicsImage::Layout::ColourAttachmentOptimal,
        };

        AttachmentReference depthReference{
            .attachmentIndex = 1,
            .layout          = GraphicsImage::Layout::DepthStencilAttachmentOptimal,
        };

        SubpassDescriptor subpass{
            .colourAttachments = { colourReference },
            .depthAttachment   = depthReference,
        };

        //Wait on the implicit subpass at the start. This is so the subpass can transition the layout at the right time
        SubpassDependency dependency{
            .sourceSubpass      = SUBPASS_EXTERNAL,
            .destinationSubpass = 0,
            .sourceStage        = PipelineObject::Stage::ColourAttachmentOutput,
            .destinationStage   = PipelineObject::Stage::ColourAttachmentOutput,
            .sourceAccess       = AccessFlags::None,
            .destinationAccess  = AccessFlags::ColourAttachmentWrite,
        };

        //Create render pass
        RenderPass::Descriptor renderPassDescriptor{
            .attachments  = { std::move(colourAttachment), std::move(depthAttachment) },
            .subpasses    = { std::move(subpass) },
            .dependencies = { std::move(dependency) },
        };

        renderPass = graphicsFactory->createRenderPass(std::move(renderPassDescriptor));
    }

    void ForwardRenderer3D::createShadowMapRenderpass() {
        AttachmentDescriptor depthAttachment{
            .format         = GraphicsImage::Format::D32_SFLOAT,
            .loadOperation  = LoadOperation::Clear,
            .storeOperation = StoreOperation::Store,
            .initialLayout  = GraphicsImage::Layout::Undefined,
            .finalLayout    = GraphicsImage::Layout::ShaderReadOnlyOptimal,
        };

        AttachmentReference depthReference{
            .attachmentIndex = 0,
            .layout          = GraphicsImage::Layout::DepthStencilAttachmentOptimal,
        };

        SubpassDescriptor subpass{
            .colourAttachments = {},
            .depthAttachment   = depthReference,
        };

        RenderPass::Descriptor renderPassDescriptor{
            .attachments  = { std::move(depthAttachment) },
            .subpasses    = { std::move(subpass) },
            .dependencies = {},
        };

        shadowMapRenderPass = graphicsFactory->createRenderPass(std::move(renderPassDescriptor));
    }

    //void ForwardRenderer3D::createCubeShadowMapRenderpass() {
    //}

    void ForwardRenderer3D::createDepthBuffer() {
        GraphicsImage::Descriptor depthDescriptor{
            .type        = GraphicsImage::Type::_2D,
            .usageFlags  = GraphicsImage::UsageMode::DepthStencilAttachment,
            .dimensions  = { swapchain->getExtent().x, swapchain->getExtent().y },
            .format      = GraphicsImage::Format::D32_SFLOAT,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::VideoMemory,
        };

        depthImage     = graphicsFactory->createImage(std::move(depthDescriptor));
        depthImageView = depthImage->createView(GraphicsImageView::Descriptor{
            .type       = GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }

    void ForwardRenderer3D::createPipeline() {
        std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayoutsVector{};
        descriptorSetLayoutsVector.reserve(std::size(descriptorSetLayouts));
        for(auto &&[key, layout] : descriptorSetLayouts) {
            descriptorSetLayoutsVector.push_back(layout);
        }

        PushConstantDescriptor modelPushConstant{
            .stage  = Shader::Stage::Vertex,
            .offset = 0,
            .size   = sizeof(clv::mth::mat4f) * 2,
        };

        PipelineObject::Descriptor pipelineDescriptor{
            .vertexShader       = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(staticmesh_v), staticmesh_vLength }),
            .fragmentShader     = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(mesh_p), mesh_pLength }),
            .vertexInput        = Vertex::getInputBindingDescriptor(),
            .vertexAttributes   = Vertex::getVertexAttributes(),
            .viewportDescriptor = {
                .size = { swapchain->getExtent().x, swapchain->getExtent().y },
            },
            .scissorDescriptor = {
                .size = { swapchain->getExtent().x, swapchain->getExtent().y },
            },
            .renderPass           = renderPass,
            .descriptorSetLayouts = std::move(descriptorSetLayoutsVector),
            .pushConstants        = { modelPushConstant },
        };

        staticMeshPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);

        pipelineDescriptor.vertexShader = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(animatedmesh_v), animatedmesh_vLength });

        animatedMeshPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);
    }

    void ForwardRenderer3D::createShadowMapPipeline() {
        PushConstantDescriptor pushConstant{
            .stage = Shader::Stage::Vertex,
            .size  = sizeof(clv::mth::mat4f) * 2,
        };

        AreaDescriptor viewScissorArea{
            .state    = ElementState::Static,
            .position = { 0.0f, 0.0f },
            .size     = { shadowMapSize, shadowMapSize }
        };

        PipelineObject::Descriptor pipelineDescriptor{
            .vertexShader         = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(staticmeshshadowmap_v), staticmeshshadowmap_vLength }),
            .fragmentShader       = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(meshshadowmap_p), meshshadowmap_pLength }),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = Vertex::getVertexAttributes(),
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .renderPass           = shadowMapRenderPass,
            .descriptorSetLayouts = {},
            .pushConstants        = { pushConstant },
        };

        staticMeshShadowMapPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);

        pipelineDescriptor.vertexShader         = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(animatedmeshshadowmap_v), animatedmeshshadowmap_vLength });
        pipelineDescriptor.descriptorSetLayouts = { descriptorSetLayouts[DescriptorSetSlots::Mesh] };

        animatedMeshShadowMapPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);
    }

    void ForwardRenderer3D::createCubeShadowMapPipeline() {
        PushConstantDescriptor vertexPushConstant{
            .stage  = Shader::Stage::Vertex,
            .offset = 0,
            .size   = sizeof(clv::mth::mat4f) * 2,
        };
        PushConstantDescriptor pixelPushConstant{
            .stage  = Shader::Stage::Pixel,
            .offset = sizeof(clv::mth::mat4f) * 2,
            .size   = sizeof(clv::mth::vec3f) + sizeof(float),
        };

        AreaDescriptor viewScissorArea{
            .state    = ElementState::Static,
            .position = { 0.0f, 0.0f },
            .size     = { shadowMapSize, shadowMapSize }
        };

        PipelineObject::Descriptor pipelineDescriptor{
            .vertexShader         = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(staticmeshcubeshadowmap_v), staticmeshcubeshadowmap_vLength }),
            .fragmentShader       = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(meshcubeshadowmap_p), meshcubeshadowmap_pLength }),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = Vertex::getVertexAttributes(),
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .renderPass           = shadowMapRenderPass,
            .descriptorSetLayouts = {},
            .pushConstants        = { vertexPushConstant, pixelPushConstant },
        };

        staticMeshCubeShadowMapPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);

        pipelineDescriptor.vertexShader         = graphicsFactory->createShader({ reinterpret_cast<std::byte const *>(animatedmeshcubeshadowmap_v), animatedmeshcubeshadowmap_vLength });
        pipelineDescriptor.descriptorSetLayouts = { descriptorSetLayouts[DescriptorSetSlots::Mesh] };

        animatedMeshCubeShadowMapPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);
    }

    void ForwardRenderer3D::createSwapchainFrameBuffers() {
        for(auto &swapChainImageView : swapchain->getImageViews()) {
            swapChainFrameBuffers.emplace_back(graphicsFactory->createFramebuffer(Framebuffer::Descriptor{
                .renderPass  = renderPass,
                .attachments = { swapChainImageView, depthImageView },
                .width       = swapchain->getExtent().x,
                .height      = swapchain->getExtent().y,
            }));
        }
    }

    std::shared_ptr<DescriptorPool> ForwardRenderer3D::createDescriptorPool(std::unordered_map<DescriptorType, uint32_t> const &bindingCount, const uint32_t setCount) {
        std::vector<DescriptorInfo> poolTypes;
        for(auto &&[type, count] : bindingCount) {
            DescriptorInfo info{
                .type  = type,
                .count = count,
            };

            poolTypes.emplace_back(info);
        }

        DescriptorPool::Descriptor poolDescriptor{
            .poolTypes = std::move(poolTypes),
            .flag      = DescriptorPool::Flag::None,
            .maxSets   = setCount,
        };

        return graphicsFactory->createDescriptorPool(std::move(poolDescriptor));
    }
}