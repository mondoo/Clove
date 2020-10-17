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
#include <Clove/Log.hpp>
#include <Clove/Platform/Window.hpp>

using namespace clv::gfx;

extern "C" const char mesh_v[];
extern "C" const size_t mesh_vLength;
extern "C" const char mesh_p[];
extern "C" const size_t mesh_pLength;

extern "C" const char genshadowmap_v[];
extern "C" const size_t genshadowmap_vLength;
extern "C" const char genshadowmap_p[];
extern "C" const size_t genshadowmap_pLength;

namespace blb::rnd {
    ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window& window, const API api) {
        windowResizeHandle = window.onWindowResize.bind(&ForwardRenderer3D::onWindowResize, this);
        windowSize         = window.getSize();

        graphicsDevice  = createGraphicsDevice(api, window.getNativeWindow());
        graphicsFactory = graphicsDevice->getGraphicsFactory();

        //Object initialisation
        graphicsQueue = graphicsFactory->createGraphicsQueue({ QueueFlags::ReuseBuffers });
        presentQueue  = graphicsFactory->createPresentQueue();
        transferQueue = graphicsFactory->createTransferQueue({ QueueFlags::Transient });

        swapchain = graphicsFactory->createSwapChain({ windowSize });
        createRenderpass();

        createDepthBuffer();

        descriptorSetLayouts = createDescriptorSetLayouts(*graphicsFactory);

        recreateSwapchain();

        //Create semaphores for frame synchronisation
        for(auto& renderFinishedSemaphore : renderFinishedSemaphores) {
            renderFinishedSemaphore = graphicsFactory->createSemaphore();
        }
        for(auto& imageAvailableSemaphore : imageAvailableSemaphores) {
            imageAvailableSemaphore = graphicsFactory->createSemaphore();
        }
        for(auto& inFlightFence : inFlightFences) {
            inFlightFence = graphicsFactory->createFence({ true });
        }
        imagesInFlight.resize(swapchain->getImageViews().size());

        Sampler::Descriptor samplerDescriptor{};
        samplerDescriptor.minFilter        = Sampler::Filter::Linear;
        samplerDescriptor.magFilter        = Sampler::Filter::Linear;
        samplerDescriptor.addressModeU     = Sampler::AddressMode::Repeat;
        samplerDescriptor.addressModeV     = Sampler::AddressMode::Repeat;
        samplerDescriptor.addressModeW     = Sampler::AddressMode::Repeat;
        samplerDescriptor.enableAnisotropy = true;
        samplerDescriptor.maxAnisotropy    = 16.0f;

        sampler = graphicsFactory->createSampler(std::move(samplerDescriptor));

        //TODO: Just making everything here until I figure out the best way to do this
        {
            //TODO: Hard coding in 3 for now (for frames in flight)
            shadowMaps.resize(3);
            shadowMapViews.resize(3);
            for(size_t i = 0; i < 3; ++i) {
                for(size_t j = 0; j < MAX_LIGHTS; ++j) {
                    //Create the shadow map used for directional lighting
                    GraphicsImage::Descriptor shadowMapDescriptor{
                        .type        = GraphicsImage::Type::_2D,
                        .usageFlags  = GraphicsImage::UsageMode::Sampled | GraphicsImage::UsageMode::DepthStencilAttachment,
                        .dimensions  = { shadowMapSize, shadowMapSize },
                        .format      = GraphicsImage::Format::D32_SFLOAT,
                        .sharingMode = SharingMode::Exclusive,
                        .memoryType  = MemoryType::VideoMemory
                    };
                    shadowMaps[i][j]     = graphicsFactory->createImage(std::move(shadowMapDescriptor));
                    shadowMapViews[i][j] = shadowMaps[i][j]->createView();
                }
                //TODO: Transition layout so it's prepped
            }

            //Render pass for generating the shadow map
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

            //Pipeline for the shadow map
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
                .vertexShader         = graphicsFactory->createShader({ reinterpret_cast<const std::byte*>(genshadowmap_v), genshadowmap_vLength }),
                .fragmentShader       = graphicsFactory->createShader({ reinterpret_cast<const std::byte*>(genshadowmap_p), genshadowmap_pLength }),
                .vertexInput          = Vertex::getInputBindingDescriptor(),
                .vertexAttributes     = Vertex::getVertexAttributes(),
                .viewportDescriptor   = viewScissorArea,
                .scissorDescriptor    = viewScissorArea,
                .renderPass           = shadowMapRenderPass,
                .descriptorSetLayouts = {},
                .pushConstants        = { pushConstant },
            };

            shadowMapPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);

            //Frame buffer for shadow map
            shadowMapFrameBuffers.resize(3);
            for(size_t i = 0; i < 3; ++i) {//TODO: Hard coding in 3 for now (for frames in flight)
                for(size_t j = 0; auto& view : shadowMapViews[i]) {
                    Framebuffer::Descriptor frameBuffer{
                        .renderPass  = shadowMapRenderPass,
                        .attachments = { view },
                        .width       = shadowMapSize,
                        .height      = shadowMapSize
                    };
                    shadowMapFrameBuffers[i][j] = graphicsFactory->createFramebuffer(frameBuffer);

                    ++j;
                }
            }
        }
    }

    //ForwardRenderer3D::ForwardRenderer3D(ForwardRenderer3D&& other) noexcept = default;

    ForwardRenderer3D& ForwardRenderer3D::operator=(ForwardRenderer3D&& other) noexcept = default;

    ForwardRenderer3D::~ForwardRenderer3D() {
        //Wait for an idle device before shutting down so resources aren't freed while in use
        graphicsDevice->waitForIdleDevice();

        //Reset these manually as they would fail after the device has been destroyed (how to solve this?)
        sampler.reset();
        for(auto& imageData : inFlightImageData) {
            imageData.uniformBuffer.reset();
            graphicsQueue->freeCommandBuffer(*imageData.commandBuffer);
        }
    }

    void ForwardRenderer3D::begin() {
        meshes.clear();
        currentFrameData.numLights.numDirectional = 0;
        currentFrameData.numLights.numPoint       = 0;
    }

    void ForwardRenderer3D::submitCamera(const Camera& camera, clv::mth::vec3f position) {
        currentFrameData.viewData.view       = camera.getView();
        currentFrameData.viewData.projection = camera.getProjection();

        currentFrameData.viewPosition = position;
    }

    void ForwardRenderer3D::submitStaticMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform) {
        meshes.push_back({ std::move(mesh), std::move(transform) });
    }

    void ForwardRenderer3D::submitAnimatedMesh(std::shared_ptr<rnd::Mesh> mesh, clv::mth::mat4f transform) {
    }

    void ForwardRenderer3D::submitLight(const DirectionalLight& light) {
        const uint32_t lightIndex = currentFrameData.numLights.numDirectional++;

        currentFrameData.lights.directionalLights[lightIndex]    = light.data;
        currentFrameData.directionalShadowTransforms[lightIndex] = light.shadowTransform;
    }

    void ForwardRenderer3D::submitLight(const PointLight& light) {
        currentFrameData.lights.pointLights[currentFrameData.numLights.numPoint++] = light.data;
    }

    void ForwardRenderer3D::submitWidget(const std::shared_ptr<Sprite>& widget) {
    }

    void ForwardRenderer3D::submitText(const std::shared_ptr<Sprite>& text) {
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

        ImageData& currentImage = inFlightImageData[imageIndex];

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

        currentImage.commandBuffer->beginRecording(CommandBufferUsage::Default);

        //SHADOW MAP - TODO: Put this into a seperate command buffer so we're not waiting on an image to be available for this step
        std::array<ClearValue, 1> shadowMapClearValues{
            ClearValue{ {}, depthStencilClearValue }
        };

        currentImage.commandBuffer->bindPipelineObject(*shadowMapPipelineObject);

        //Directional lights
        for(size_t i = 0; i < MAX_LIGHTS; ++i) {
            //Make sure to begin the render pass on the images we don't draw to so their layout is transitioned properly
            currentImage.commandBuffer->beginRenderPass(*shadowMapRenderPass, *shadowMapFrameBuffers[imageIndex][i], shadowArea, shadowMapClearValues);

            if(i < currentFrameData.numLights.numDirectional) {
                for(auto&& [mesh, transform] : meshes) {
                    const clv::mth::mat4f pushConstantData[]{ transform, currentFrameData.directionalShadowTransforms[i] };

                    currentImage.commandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
                    currentImage.commandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

                    currentImage.commandBuffer->pushConstant(*shadowMapPipelineObject, Shader::Stage::Vertex, sizeof(pushConstantData), pushConstantData);

                    currentImage.commandBuffer->drawIndexed(mesh->getIndexCount());
                }
            }

            currentImage.commandBuffer->endRenderPass();
        }

        //FINAL IMAGE
        std::array<ClearValue, 2> outputClearValues{
            ClearValue{ clearColour, {} },
            ClearValue{ {}, depthStencilClearValue }
        };

        currentImage.commandBuffer->beginRenderPass(*renderPass, *swapChainFrameBuffers[imageIndex], renderArea, outputClearValues);
        currentImage.commandBuffer->bindPipelineObject(*pipelineObject);

        //We can just write the struct straight in as all the mappings are based off of it's layout
        currentImage.uniformBuffer->write(&currentFrameData, 0, sizeof(currentFrameData));

        //Map any non-UBO pieces of data (such as textures / shadow maps)
        currentImage.lightingDescriptorSet->map(shadowMapViews[imageIndex], *sampler, GraphicsImage::Layout::ShaderReadOnlyOptimal, 3);

        //Bind the descriptor sets that all objects will use
        currentImage.commandBuffer->bindDescriptorSet(*currentImage.viewDescriptorSet, *pipelineObject, static_cast<uint32_t>(DescriptorSetSlots::View));
        currentImage.commandBuffer->bindDescriptorSet(*currentImage.lightingDescriptorSet, *pipelineObject, static_cast<uint32_t>(DescriptorSetSlots::Lighting));

        size_t const meshCount = std::size(meshes);

        //Allocate a descriptor set for each mesh to be drawn
        if(currentImage.materialDescriptorPool == nullptr || currentImage.materialDescriptorPool->getDescriptor().maxSets < meshCount) {
            auto materialSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::Material]);
            for(auto& [key, val] : materialSetBindingCount) {
                val *= meshCount;
            }
            currentImage.materialDescriptorPool = createDescriptorPool(materialSetBindingCount, meshCount);
        }

        currentImage.materialDescriptorPool->reset();
        std::vector<std::shared_ptr<DescriptorSetLayout>> layouts(meshCount, descriptorSetLayouts[DescriptorSetSlots::Material]);
        std::vector<std::shared_ptr<DescriptorSet>> materialSets = currentImage.materialDescriptorPool->allocateDescriptorSets(layouts);

        //Bind all mesh data
        size_t meshIndex = 0;
        for(auto&& [mesh, transform] : meshes) {
            std::shared_ptr<DescriptorSet>& materialDescriptorSet = materialSets[meshIndex];

            VertexData modelData{};
            modelData.model        = transform;
            modelData.normalMatrix = clv::mth::inverse(clv::mth::transpose(transform));

            materialDescriptorSet->map(*mesh->getMaterial().diffuseView, *sampler, GraphicsImage::Layout::ShaderReadOnlyOptimal, 0);

            currentImage.commandBuffer->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
            currentImage.commandBuffer->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

            currentImage.commandBuffer->bindDescriptorSet(*materialDescriptorSet, *pipelineObject, 0);//TODO: Get correct setNum
            currentImage.commandBuffer->pushConstant(*pipelineObject, Shader::Stage::Vertex, sizeof(VertexData), &modelData);

            currentImage.commandBuffer->drawIndexed(mesh->getIndexCount());

            ++meshIndex;
        }

        currentImage.commandBuffer->endRenderPass();

        currentImage.commandBuffer->endRecording();

        //Submit the command buffer associated with that image
        GraphicsSubmitInfo submitInfo{};
        submitInfo.waitSemaphores   = { imageAvailableSemaphores[currentFrame] };
        submitInfo.waitStages       = { PipelineObject::Stage::ColourAttachmentOutput };
        submitInfo.commandBuffers   = { currentImage.commandBuffer };
        submitInfo.signalSemaphores = { renderFinishedSemaphores[currentFrame] };
        graphicsQueue->submit(submitInfo, inFlightFences[currentFrame].get());

        //Present current image
        PresentInfo presentInfo{};
        presentInfo.waitSemaphores = { renderFinishedSemaphores[currentFrame] };
        presentInfo.swapChain      = swapchain;
        presentInfo.imageIndex     = imageIndex;
        result                     = presentQueue->present(presentInfo);

        if(needNewSwapchain || result == Result::Error_SwapchainOutOfDate || result == Result::Success_SwapchainSuboptimal) {
            recreateSwapchain();
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "Swapchain recreated at end of loop");
        }

        currentFrame = (currentFrame + 1) % maxFramesInFlight;
    }

    const std::shared_ptr<GraphicsFactory>& ForwardRenderer3D::getGraphicsFactory() const {
        return graphicsFactory;
    }

    void ForwardRenderer3D::onWindowResize(const clv::mth::vec2ui& size) {
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
        pipelineObject.reset();
        swapChainFrameBuffers.clear();
        for(auto& imageData : inFlightImageData) {
            graphicsQueue->freeCommandBuffer(*imageData.commandBuffer);
        }

        //Recreate our swap chain
        swapchain = graphicsFactory->createSwapChain({ windowSize });

        createRenderpass();

        createDepthBuffer();

        createPipeline();
        createSwapchainFrameBuffers();

        const size_t imageCount = std::size(swapChainFrameBuffers);

        inFlightImageData.resize(imageCount);

        //Allocate frame scope descriptor pools
        auto viewSetBindingCount     = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::View]);
        auto lightingSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[DescriptorSetSlots::Lighting]);

        uint32_t constexpr totalSets{ 2 };//Only 2 sets will be allocated from these pools (view + lighting)
        auto bindingCounts = viewSetBindingCount;
        bindingCounts.merge(lightingSetBindingCount);

        for(auto& imageData : inFlightImageData) {
            //Create command buffers
            imageData.commandBuffer = graphicsQueue->allocateCommandBuffer();

            //Create uniform buffers
            imageData.uniformBuffer = createUniformBuffer();

            //Allocate frame scope descriptor Sets
            imageData.frameDescriptorPool = createDescriptorPool(bindingCounts, totalSets);

            imageData.viewDescriptorSet     = imageData.frameDescriptorPool->allocateDescriptorSets(descriptorSetLayouts[DescriptorSetSlots::View]);
            imageData.lightingDescriptorSet = imageData.frameDescriptorPool->allocateDescriptorSets(descriptorSetLayouts[DescriptorSetSlots::Lighting]);

            //As we only have one UBO per frame for every DescriptorSet we can map the buffer into them straight away
            imageData.viewDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData, viewData), sizeof(currentFrameData.viewData), 0);
            imageData.viewDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData, viewPosition), sizeof(currentFrameData.viewPosition), 1);

            imageData.lightingDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData, lights), sizeof(currentFrameData.lights), 0);
            imageData.lightingDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData, numLights), sizeof(currentFrameData.numLights), 1);
            imageData.lightingDescriptorSet->map(*imageData.uniformBuffer, offsetof(FrameData, directionalShadowTransforms), sizeof(currentFrameData.directionalShadowTransforms), 2);
        }

        needNewSwapchain = false;
    }

    void ForwardRenderer3D::createRenderpass() {
        //Define what attachments we have
        AttachmentDescriptor colourAttachment{};
        colourAttachment.format         = swapchain->getImageFormat();
        colourAttachment.loadOperation  = LoadOperation::Clear;
        colourAttachment.storeOperation = StoreOperation::Store;
        colourAttachment.initialLayout  = GraphicsImage::Layout::Undefined;
        colourAttachment.finalLayout    = GraphicsImage::Layout::Present;

        AttachmentDescriptor depthAttachment{};
        depthAttachment.format         = GraphicsImage::Format::D32_SFLOAT;
        depthAttachment.loadOperation  = LoadOperation::Clear;
        depthAttachment.storeOperation = StoreOperation::DontCare;
        depthAttachment.initialLayout  = GraphicsImage::Layout::Undefined;
        depthAttachment.finalLayout    = GraphicsImage::Layout::DepthStencilAttachmentOptimal;

        //Define attachment references so the subpass knows which slot each attachment will be in
        AttachmentReference colourReference{};
        colourReference.attachmentIndex = 0;
        colourReference.layout          = GraphicsImage::Layout::ColourAttachmentOptimal;

        AttachmentReference depthReference{};
        depthReference.attachmentIndex = 1;
        depthReference.layout          = GraphicsImage::Layout::DepthStencilAttachmentOptimal;

        SubpassDescriptor subpass{};
        subpass.colourAttachments = { colourReference };
        subpass.depthAttachment   = depthReference;

        //Wait on the implicit subpass at the start. This is so the subpass can transition the layout at the right time
        SubpassDependency dependency{};
        dependency.sourceSubpass      = SUBPASS_EXTERNAL;
        dependency.destinationSubpass = 0;
        dependency.sourceStage        = PipelineObject::Stage::ColourAttachmentOutput;
        dependency.destinationStage   = PipelineObject::Stage::ColourAttachmentOutput;
        dependency.sourceAccess       = AccessFlags::None;
        dependency.destinationAccess  = AccessFlags::ColourAttachmentWrite;

        //Create render pass
        RenderPass::Descriptor renderPassDescriptor{};
        renderPassDescriptor.attachments  = { std::move(colourAttachment), std::move(depthAttachment) };
        renderPassDescriptor.subpasses    = { std::move(subpass) };
        renderPassDescriptor.dependencies = { std::move(dependency) };

        renderPass = graphicsFactory->createRenderPass(std::move(renderPassDescriptor));
    }

    void ForwardRenderer3D::createDepthBuffer() {
        GraphicsImage::Descriptor depthDescriptor{};
        depthDescriptor.type        = GraphicsImage::Type::_2D;
        depthDescriptor.usageFlags  = GraphicsImage::UsageMode::DepthStencilAttachment;
        depthDescriptor.dimensions  = { swapchain->getExtent().x, swapchain->getExtent().y };
        depthDescriptor.format      = GraphicsImage::Format::D32_SFLOAT;
        depthDescriptor.sharingMode = SharingMode::Exclusive;
        depthDescriptor.memoryType  = MemoryType::VideoMemory;

        depthImage     = graphicsFactory->createImage(std::move(depthDescriptor));
        depthImageView = depthImage->createView();
    }

    void ForwardRenderer3D::createPipeline() {
        std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayoutsVector{};
        descriptorSetLayoutsVector.reserve(std::size(descriptorSetLayouts));
        for(auto&& [key, layout] : descriptorSetLayouts) {
            descriptorSetLayoutsVector.push_back(layout);
        }

        PushConstantDescriptor modelPushConstant{
            .stage = Shader::Stage::Vertex,
            .size  = sizeof(VertexData),
        };

        PipelineObject::Descriptor pipelineDescriptor{
            .vertexShader       = graphicsFactory->createShader({ reinterpret_cast<std::byte const*>(mesh_v), mesh_vLength }),
            .fragmentShader     = graphicsFactory->createShader({ reinterpret_cast<std::byte const*>(mesh_p), mesh_pLength }),
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

        pipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);
    }

    void ForwardRenderer3D::createSwapchainFrameBuffers() {
        for(auto& swapChainImageView : swapchain->getImageViews()) {
            Framebuffer::Descriptor frameBufferDescriptor{};
            frameBufferDescriptor.renderPass  = renderPass;
            frameBufferDescriptor.attachments = { swapChainImageView, depthImageView };
            frameBufferDescriptor.width       = swapchain->getExtent().x;
            frameBufferDescriptor.height      = swapchain->getExtent().y;

            swapChainFrameBuffers.emplace_back(graphicsFactory->createFramebuffer(frameBufferDescriptor));
        }
    }

    std::shared_ptr<GraphicsBuffer> ForwardRenderer3D::createUniformBuffer() {
        GraphicsBuffer::Descriptor descriptor{
            .size        = sizeof(FrameData),
            .usageFlags  = GraphicsBuffer::UsageMode::UniformBuffer,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        };
        return graphicsFactory->createBuffer(std::move(descriptor));
    }

    std::shared_ptr<DescriptorPool> ForwardRenderer3D::createDescriptorPool(const std::unordered_map<DescriptorType, uint32_t>& bindingCount, const uint32_t setCount) {
        std::vector<DescriptorInfo> poolTypes;
        for(auto&& [type, count] : bindingCount) {
            DescriptorInfo info{};
            info.type  = type;
            info.count = count;

            poolTypes.emplace_back(info);
        }

        DescriptorPool::Descriptor poolDescriptor{};
        poolDescriptor.poolTypes = std::move(poolTypes);
        poolDescriptor.flag      = DescriptorPool::Flag::None;
        poolDescriptor.maxSets   = setCount;

        return graphicsFactory->createDescriptorPool(std::move(poolDescriptor));
    }
}