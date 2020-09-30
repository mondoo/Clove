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

        //Define the layout of the buffer backing the uniform object buffer
        const size_t minOffUniformBufferAlignment = graphicsDevice->getLimits().minUniformBufferOffsetAlignment;

        const auto getByteBoundary = [minOffUniformBufferAlignment](const size_t currentOffset) {
            return minOffUniformBufferAlignment - (currentOffset % minOffUniformBufferAlignment);
        };

        uniformBufferLayout.viewSize      = sizeof(ViewData);
        uniformBufferLayout.viewPosSize   = sizeof(ViewData);
        uniformBufferLayout.lightSize     = sizeof(LightDataArray);
        uniformBufferLayout.numLightsSize = sizeof(LightCount);

        uniformBufferLayout.viewOffset      = 0;
        uniformBufferLayout.viewPosOffset   = uniformBufferLayout.viewSize + getByteBoundary(uniformBufferLayout.viewSize);
        uniformBufferLayout.lightOffset     = (uniformBufferLayout.viewPosOffset + uniformBufferLayout.viewPosSize) + getByteBoundary(uniformBufferLayout.viewPosOffset + uniformBufferLayout.viewPosSize);
        uniformBufferLayout.numLightsOffset = (uniformBufferLayout.lightOffset + uniformBufferLayout.lightSize) + getByteBoundary(uniformBufferLayout.lightOffset + uniformBufferLayout.lightSize);

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
            //TODO: Hard coding in 3 for now
            shadowMaps.resize(3);
            shadowMapViews.resize(3);
            for(size_t i = 0; i < 3; ++i) {
                //Create the shadow map used for directional lighting
                GraphicsImage::Descriptor shadowMapDescriptor{
                    .type        = GraphicsImage::Type::_2D,
                    .usageFlags  = GraphicsImage::UsageMode::Sampled | GraphicsImage::UsageMode::DepthStencilAttachment,
                    .dimensions  = { shadowMapSize, shadowMapSize },
                    .format      = ImageFormat::D32_SFLOAT,
                    .sharingMode = SharingMode::Exclusive,
                    .memoryType  = MemoryType::VideoMemory
                };
                shadowMaps[i]     = graphicsFactory->createImage(std::move(shadowMapDescriptor));
                shadowMapViews[i] = shadowMaps[i]->createView();
                //TODO: Transition layout so it's prepped
            }

            //Render pass for generating the shadow map
            AttachmentDescriptor depthAttachment{
                .format         = ImageFormat::D32_SFLOAT,
                .loadOperation  = LoadOperation::Clear,
                .storeOperation = StoreOperation::DontCare,
                .initialLayout  = ImageLayout::Undefined,
                .finalLayout    = ImageLayout::ShaderReadOnlyOptimal,
            };

            AttachmentReference depthReference{
                .attachmentIndex = 0,
                .layout          = ImageLayout::DepthStencilAttachmentOptimal,
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
            PushConstantDescriptor modelPushConstant{
                .stage = Shader::Stage::Vertex,
                .size  = sizeof(VertexData),
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
                .descriptorSetLayouts = descriptorSetLayouts,//TODO: Using all of the possible layouts. Is it better to only define the ones it needs?
                .pushConstants        = { modelPushConstant },
            };

            shadowMapPipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);

            //Frame buffer for shadow map
            shadowMapFrameBuffers.reserve(std::size(shadowMapViews));
            for(auto& view : shadowMapViews) {
                Framebuffer::Descriptor frameBuffer{
                    .renderPass  = shadowMapRenderPass,
                    .attachments = { view },
                    .width = shadowMapSize,
                    .height = shadowMapSize
                };
                shadowMapFrameBuffers.emplace_back(graphicsFactory->createFramebuffer(frameBuffer));
            }
        }
    }

    ForwardRenderer3D::ForwardRenderer3D(ForwardRenderer3D&& other) noexcept = default;

    ForwardRenderer3D& ForwardRenderer3D::operator=(ForwardRenderer3D&& other) noexcept = default;

    ForwardRenderer3D::~ForwardRenderer3D() {
        //Wait for an idle device before shutting down so resources aren't freed while in use
        graphicsDevice->waitForIdleDevice();

        //Reset these manually as they would fail after the device has been destroyed (how to solve this?)
        uniformBuffers.clear();
        sampler.reset();
        for(auto& buffer : commandBuffers) {
            graphicsQueue->freeCommandBuffer(*buffer);
        }
        commandBuffers.clear();
    }

    void ForwardRenderer3D::begin() {
        currentFrameData.meshes.clear();
        currentFrameData.numLights.numDirectional = 0;
        currentFrameData.numLights.numPoint       = 0;
    }

    void ForwardRenderer3D::submitCamera(const Camera& camera, clv::mth::vec3f position) {
        currentFrameData.view       = camera.getView();
        currentFrameData.projection = camera.getProjection();

        currentFrameData.viewPosition = position;
    }

    void ForwardRenderer3D::submitStaticMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform) {
        currentFrameData.meshes.push_back({ std::move(mesh), std::move(transform) });
    }

    void ForwardRenderer3D::submitAnimatedMesh(std::shared_ptr<rnd::Mesh> mesh, clv::mth::mat4f transform) {
    }

    void ForwardRenderer3D::submitLight(const DirectionalLight& light) {
        const uint32_t lightIndex = currentFrameData.numLights.numDirectional++;

        currentFrameData.lights.directionalLights[lightIndex] = light.data;
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

        //Record our command buffers
        RenderArea renderArea{};
        renderArea.origin = { 0, 0 };
        renderArea.size   = { swapchain->getExtent().x, swapchain->getExtent().y };

        clv::mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };
        DepthStencilValue depthStencilClearValue{ 1.0f, 0 };

        commandBuffers[imageIndex]->beginRecording(CommandBufferUsage::Default);
        commandBuffers[imageIndex]->beginRenderPass(*renderPass, *swapChainFrameBuffers[imageIndex], renderArea, clearColour, depthStencilClearValue);
        commandBuffers[imageIndex]->bindPipelineObject(*pipelineObject);

        //Map / Bind the descriptor sets for the per frame data
        ViewData viewData{};
        viewData.view       = currentFrameData.view;
        viewData.projection = currentFrameData.projection;

        uniformBuffers[imageIndex]->map(&viewData, uniformBufferLayout.viewOffset, uniformBufferLayout.viewSize);
        uniformBuffers[imageIndex]->map(&currentFrameData.viewPosition, uniformBufferLayout.viewPosOffset, uniformBufferLayout.viewPosSize);

        uniformBuffers[imageIndex]->map(&currentFrameData.lights, uniformBufferLayout.lightOffset, uniformBufferLayout.lightSize);
        uniformBuffers[imageIndex]->map(&currentFrameData.numLights, uniformBufferLayout.numLightsOffset, uniformBufferLayout.numLightsSize);

        commandBuffers[imageIndex]->bindDescriptorSet(*descriptorSets[imageIndex].viewSet, *pipelineObject, static_cast<uint32_t>(DescriptorSetSlots::View));
        commandBuffers[imageIndex]->bindDescriptorSet(*descriptorSets[imageIndex].lightingSet, *pipelineObject, static_cast<uint32_t>(DescriptorSetSlots::Lighting));

        const auto materialIndex = static_cast<size_t>(DescriptorSetSlots::Material);
        const size_t meshCount   = std::size(currentFrameData.meshes);

        //Allocate a descriptor set for each mesh to be drawn
        if(materialDescriptorPool[imageIndex] == nullptr || materialDescriptorPool[imageIndex]->getDescriptor().maxSets < meshCount) {
            auto materialSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[materialIndex]);
            for(auto& [key, val] : materialSetBindingCount) {
                val *= meshCount;
            }
            materialDescriptorPool[imageIndex] = createDescriptorPool(materialSetBindingCount, meshCount);
        }

        materialDescriptorPool[imageIndex]->reset();
        std::vector<std::shared_ptr<DescriptorSetLayout>> layouts(meshCount, descriptorSetLayouts[materialIndex]);
        std::vector<std::shared_ptr<DescriptorSet>> materialSets = materialDescriptorPool[imageIndex]->allocateDescriptorSets(layouts);

        //Bind all mesh data
        size_t meshIndex = 0;
        for(auto&& [mesh, transform] : currentFrameData.meshes) {
            std::shared_ptr<DescriptorSet>& materialDescriptorSet = materialSets[meshIndex];

            VertexData modelData{};
            modelData.model        = transform;
            modelData.normalMatrix = clv::mth::inverse(clv::mth::transpose(transform));

            materialDescriptorSet->write(*mesh->getMaterial().diffuseView, *sampler, ImageLayout::ShaderReadOnlyOptimal, 0);

            commandBuffers[imageIndex]->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
            commandBuffers[imageIndex]->bindIndexBuffer(*mesh->getIndexBuffer(), IndexType::Uint16);

            commandBuffers[imageIndex]->bindDescriptorSet(*materialDescriptorSet, *pipelineObject, 0);//TODO: Get correct setNum
            commandBuffers[imageIndex]->pushConstant(*pipelineObject, Shader::Stage::Vertex, sizeof(VertexData), &modelData);

            commandBuffers[imageIndex]->drawIndexed(mesh->getIndexCount());

            ++meshIndex;
        }

        commandBuffers[imageIndex]->endRenderPass();
        commandBuffers[imageIndex]->endRecording();

        //Submit the command buffer associated with that image
        GraphicsSubmitInfo submitInfo{};
        submitInfo.waitSemaphores   = { imageAvailableSemaphores[currentFrame] };
        submitInfo.waitStages       = { WaitStage::ColourAttachmentOutput };
        submitInfo.commandBuffers   = { commandBuffers[imageIndex] };
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
        for(auto& buffer : commandBuffers) {
            graphicsQueue->freeCommandBuffer(*buffer);
        }
        commandBuffers.clear();

        //Recreate our swap chain
        swapchain = graphicsFactory->createSwapChain({ windowSize });

        createRenderpass();

        createDepthBuffer();

        createPipeline();
        createSwapchainFrameBuffers();

        const size_t imageCount = std::size(swapChainFrameBuffers);

        uniformBuffers = createUniformBuffers(imageCount);

        materialDescriptorPool.resize(imageCount);
        frameDescriptorPool.resize(imageCount);
        descriptorSets.resize(imageCount);

        //Allocate frame scope descriptor pools
        const auto viewIndex     = static_cast<size_t>(DescriptorSetSlots::View);
        const auto lightingIndex = static_cast<size_t>(DescriptorSetSlots::Lighting);

        auto viewSetBindingCount     = countDescriptorBindingTypes(*descriptorSetLayouts[viewIndex]);
        auto lightingSetBindingCount = countDescriptorBindingTypes(*descriptorSetLayouts[static_cast<size_t>(DescriptorSetSlots::Lighting)]);

        constexpr uint32_t totalSets{ 2 };//Only 2 sets will be allocated from these pools
        auto bindingCounts = viewSetBindingCount;
        bindingCounts.merge(lightingSetBindingCount);

        commandBuffers.reserve(imageCount);

        for(size_t i = 0; i < imageCount; ++i) {
            //Create command buffers
            commandBuffers.emplace_back(graphicsQueue->allocateCommandBuffer());

            //Allocate frame scope descriptor Sets
            frameDescriptorPool[i] = createDescriptorPool(bindingCounts, totalSets);

            descriptorSets[i].viewSet     = frameDescriptorPool[i]->allocateDescriptorSets(descriptorSetLayouts[viewIndex]);
            descriptorSets[i].lightingSet = frameDescriptorPool[i]->allocateDescriptorSets(descriptorSetLayouts[lightingIndex]);

            //As we only have one UBO per frame for every DescriptorSet we can write the buffer into them straight away
            descriptorSets[i].viewSet->write(*uniformBuffers[i], uniformBufferLayout.viewOffset, uniformBufferLayout.viewSize, 0);
            descriptorSets[i].viewSet->write(*uniformBuffers[i], uniformBufferLayout.viewPosOffset, uniformBufferLayout.viewPosSize, 1);

            descriptorSets[i].lightingSet->write(*uniformBuffers[i], uniformBufferLayout.lightOffset, uniformBufferLayout.lightSize, 0);
            descriptorSets[i].lightingSet->write(*uniformBuffers[i], uniformBufferLayout.numLightsOffset, uniformBufferLayout.numLightsSize, 1);
        }

        needNewSwapchain = false;
    }

    void ForwardRenderer3D::createRenderpass() {
        //Define what attachments we have
        AttachmentDescriptor colourAttachment{};
        colourAttachment.format         = swapchain->getImageFormat();
        colourAttachment.loadOperation  = LoadOperation::Clear;
        colourAttachment.storeOperation = StoreOperation::Store;
        colourAttachment.initialLayout  = ImageLayout::Undefined;
        colourAttachment.finalLayout    = ImageLayout::Present;

        AttachmentDescriptor depthAttachment{};
        depthAttachment.format         = ImageFormat::D32_SFLOAT;
        depthAttachment.loadOperation  = LoadOperation::Clear;
        depthAttachment.storeOperation = StoreOperation::DontCare;
        depthAttachment.initialLayout  = ImageLayout::Undefined;
        depthAttachment.finalLayout    = ImageLayout::DepthStencilAttachmentOptimal;

        //Define attachment references so the subpass knows which slot each attachment will be in
        AttachmentReference colourReference{};
        colourReference.attachmentIndex = 0;
        colourReference.layout          = ImageLayout::ColourAttachmentOptimal;

        AttachmentReference depthReference{};
        depthReference.attachmentIndex = 1;
        depthReference.layout          = ImageLayout::DepthStencilAttachmentOptimal;

        SubpassDescriptor subpass{};
        subpass.colourAttachments = { colourReference };
        subpass.depthAttachment   = depthReference;

        //Wait on the implicit subpass at the start. This is so the subpass can transition the layout at the right time
        SubpassDependency dependency{};
        dependency.sourceSubpass      = SUBPASS_EXTERNAL;
        dependency.destinationSubpass = 0;
        dependency.sourceStage        = PipelineStage::ColourAttachmentOutput;
        dependency.destinationStage   = PipelineStage::ColourAttachmentOutput;
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
        depthDescriptor.format      = ImageFormat::D32_SFLOAT;
        depthDescriptor.sharingMode = SharingMode::Exclusive;
        depthDescriptor.memoryType  = MemoryType::VideoMemory;

        depthImage     = graphicsFactory->createImage(std::move(depthDescriptor));
        depthImageView = depthImage->createView();
    }

    void ForwardRenderer3D::createPipeline() {
        PushConstantDescriptor modelPushConstant{};
        modelPushConstant.stage = Shader::Stage::Vertex;
        modelPushConstant.size  = sizeof(VertexData);

        PipelineObject::Descriptor pipelineDescriptor;
        pipelineDescriptor.vertexShader            = graphicsFactory->createShader({ reinterpret_cast<const std::byte*>(mesh_v), mesh_vLength });
        pipelineDescriptor.fragmentShader          = graphicsFactory->createShader({ reinterpret_cast<const std::byte*>(mesh_p), mesh_pLength });
        pipelineDescriptor.vertexInput             = Vertex::getInputBindingDescriptor();
        pipelineDescriptor.vertexAttributes        = Vertex::getVertexAttributes();
        pipelineDescriptor.viewportDescriptor.size = { swapchain->getExtent().x, swapchain->getExtent().y };
        pipelineDescriptor.scissorDescriptor.size  = { swapchain->getExtent().x, swapchain->getExtent().y };
        pipelineDescriptor.renderPass              = renderPass;
        pipelineDescriptor.descriptorSetLayouts    = descriptorSetLayouts;
        pipelineDescriptor.pushConstants           = { modelPushConstant };

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

    std::vector<std::shared_ptr<GraphicsBuffer>> ForwardRenderer3D::createUniformBuffers(const uint32_t bufferCount) {
        std::vector<std::shared_ptr<GraphicsBuffer>> uniformBuffers(bufferCount);

        const size_t minOffUniformBufferAlignment = graphicsDevice->getLimits().minUniformBufferOffsetAlignment;
        const size_t padding                      = minOffUniformBufferAlignment * 2;//Add padding so that each element has room to be aligned

        for(size_t i = 0; i < bufferCount; ++i) {
            GraphicsBuffer::Descriptor descriptor{};
            descriptor.size        = uniformBufferLayout.totalSize() + padding;
            descriptor.usageFlags  = GraphicsBuffer::UsageMode::UniformBuffer;
            descriptor.sharingMode = SharingMode::Exclusive;
            descriptor.memoryType  = MemoryType::SystemMemory;

            uniformBuffers[i] = graphicsFactory->createBuffer(std::move(descriptor));
        }

        return uniformBuffers;
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