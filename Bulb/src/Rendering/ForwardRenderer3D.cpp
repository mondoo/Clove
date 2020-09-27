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
#include <Clove/Log.hpp>

extern "C" const char mesh_v[];
extern "C" const size_t mesh_vLength;
extern "C" const char mesh_p[];
extern "C" const size_t mesh_pLength;

namespace blb::rnd {
    ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API api) {
        windowResizeHandle = window.onWindowResize.bind(&ForwardRenderer3D::onWindowResize, this);
        windowSize         = window.getSize();

        graphicsDevice  = clv::gfx::createGraphicsDevice(api, window.getNativeWindow());
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
        graphicsQueue = graphicsFactory->createGraphicsQueue({ clv::gfx::QueueFlags::ReuseBuffers });
        presentQueue  = graphicsFactory->createPresentQueue();
        transferQueue = graphicsFactory->createTransferQueue({ clv::gfx::QueueFlags::Transient });

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

        clv::gfx::Sampler::Descriptor samplerDescriptor{};
        samplerDescriptor.minFilter        = clv::gfx::Sampler::Filter::Linear;
        samplerDescriptor.magFilter        = clv::gfx::Sampler::Filter::Linear;
        samplerDescriptor.addressModeU     = clv::gfx::Sampler::AddressMode::Repeat;
        samplerDescriptor.addressModeV     = clv::gfx::Sampler::AddressMode::Repeat;
        samplerDescriptor.addressModeW     = clv::gfx::Sampler::AddressMode::Repeat;
        samplerDescriptor.enableAnisotropy = true;
        samplerDescriptor.maxAnisotropy    = 16.0f;

        sampler = graphicsFactory->createSampler(std::move(samplerDescriptor));
    }

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
        currentFrameData.numLights.numPoint = 0;
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
        currentFrameData.lights.directionalLights[currentFrameData.numLights.numDirectional++] = light.data;
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
        clv::gfx::Result result = swapchain->aquireNextImage(imageAvailableSemaphores[currentFrame].get(), imageIndex);
        if(result == clv::gfx::Result::Error_SwapchainOutOfDate) {
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
        clv::gfx::RenderArea renderArea{};
        renderArea.origin = { 0, 0 };
        renderArea.size   = { swapchain->getExtent().x, swapchain->getExtent().y };

        clv::mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };
        clv::gfx::DepthStencilValue depthStencilClearValue{ 1.0f, 0 };

        commandBuffers[imageIndex]->beginRecording(clv::gfx::CommandBufferUsage::Default);
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
        std::vector<std::shared_ptr<clv::gfx::DescriptorSetLayout>> layouts(meshCount, descriptorSetLayouts[materialIndex]);
        std::vector<std::shared_ptr<clv::gfx::DescriptorSet>> materialSets = materialDescriptorPool[imageIndex]->allocateDescriptorSets(layouts);

        //Bind all mesh data
        size_t meshIndex = 0;
        for(auto&& [mesh, transform] : currentFrameData.meshes) {
            std::shared_ptr<clv::gfx::DescriptorSet>& materialDescriptorSet = materialSets[meshIndex];

            VertexData modelData{};
            modelData.model        = transform;
            modelData.normalMatrix = clv::mth::inverse(clv::mth::transpose(transform));

            materialDescriptorSet->write(*mesh->getMaterial().diffuseView, *sampler, clv::gfx::ImageLayout::ShaderReadOnlyOptimal, 0);

            commandBuffers[imageIndex]->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
            commandBuffers[imageIndex]->bindIndexBuffer(*mesh->getIndexBuffer(), clv::gfx::IndexType::Uint16);

            commandBuffers[imageIndex]->bindDescriptorSet(*materialDescriptorSet, *pipelineObject, 0);//TODO: Get correct setNum
            commandBuffers[imageIndex]->pushConstant(*pipelineObject, clv::gfx::ShaderStage::Vertex, sizeof(VertexData), &modelData);

            commandBuffers[imageIndex]->drawIndexed(mesh->getIndexCount());

            ++meshIndex;
        }

        commandBuffers[imageIndex]->endRenderPass();
        commandBuffers[imageIndex]->endRecording();

        //Submit the command buffer associated with that image
        clv::gfx::GraphicsSubmitInfo submitInfo{};
        submitInfo.waitSemaphores   = { imageAvailableSemaphores[currentFrame] };
        submitInfo.waitStages       = { clv::gfx::WaitStage::ColourAttachmentOutput };
        submitInfo.commandBuffers   = { commandBuffers[imageIndex] };
        submitInfo.signalSemaphores = { renderFinishedSemaphores[currentFrame] };
        graphicsQueue->submit(submitInfo, inFlightFences[currentFrame].get());

        //Present current image
        clv::gfx::PresentInfo presentInfo{};
        presentInfo.waitSemaphores = { renderFinishedSemaphores[currentFrame] };
        presentInfo.swapChain      = swapchain;
        presentInfo.imageIndex     = imageIndex;
        result                     = presentQueue->present(presentInfo);

        if(needNewSwapchain || result == clv::gfx::Result::Error_SwapchainOutOfDate || result == clv::gfx::Result::Success_SwapchainSuboptimal) {
            recreateSwapchain();
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "Swapchain recreated at end of loop");
        }

        currentFrame = (currentFrame + 1) % maxFramesInFlight;
    }

    const std::shared_ptr<clv::gfx::GraphicsFactory>& ForwardRenderer3D::getGraphicsFactory() const {
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
        clv::gfx::AttachmentDescriptor colourAttachment{};
        colourAttachment.format         = swapchain->getImageFormat();
        colourAttachment.loadOperation  = clv::gfx::LoadOperation::Clear;
        colourAttachment.storeOperation = clv::gfx::StoreOperation::Store;
        colourAttachment.initialLayout  = clv::gfx::ImageLayout::Undefined;
        colourAttachment.finalLayout    = clv::gfx::ImageLayout::Present;

        clv::gfx::AttachmentDescriptor depthAttachment{};
        depthAttachment.format         = clv::gfx::ImageFormat::D32_SFLOAT;
        depthAttachment.loadOperation  = clv::gfx::LoadOperation::Clear;
        depthAttachment.storeOperation = clv::gfx::StoreOperation::DontCare;
        depthAttachment.initialLayout  = clv::gfx::ImageLayout::Undefined;
        depthAttachment.finalLayout    = clv::gfx::ImageLayout::DepthStencilAttachmentOptimal;

        //Define attachment references so the subpass knows which slot each attachment will be in
        clv::gfx::AttachmentReference colourReference{};
        colourReference.attachmentIndex = 0;
        colourReference.layout          = clv::gfx::ImageLayout::ColourAttachmentOptimal;

        clv::gfx::AttachmentReference depthReference{};
        depthReference.attachmentIndex = 1;
        depthReference.layout          = clv::gfx::ImageLayout::DepthStencilAttachmentOptimal;

        clv::gfx::SubpassDescriptor subpass{};
        subpass.colourAttachments = { colourReference };
        subpass.depthAttachment   = depthReference;

        //Wait on the implicit subpass at the start so we transition our image when we have one
        clv::gfx::SubpassDependency dependency{};
        dependency.sourceSubpass      = clv::gfx::SUBPASS_EXTERNAL;
        dependency.destinationSubpass = 0;
        dependency.sourceStage        = clv::gfx::PipelineStage::ColourAttachmentOutput;
        dependency.destinationStage   = clv::gfx::PipelineStage::ColourAttachmentOutput;
        dependency.sourceAccess       = clv::gfx::AccessFlags::None;
        dependency.destinationAccess  = clv::gfx::AccessFlags::ColourAttachmentWrite;

        //Create render pass
        clv::gfx::RenderPass::Descriptor renderPassDescriptor{};
        renderPassDescriptor.attachments  = { std::move(colourAttachment), std::move(depthAttachment) };
        renderPassDescriptor.subpasses    = { std::move(subpass) };
        renderPassDescriptor.dependencies = { std::move(dependency) };

        renderPass = graphicsFactory->createRenderPass(std::move(renderPassDescriptor));
    }

    void ForwardRenderer3D::createDepthBuffer() {
        clv::gfx::GraphicsImage::Descriptor depthDescriptor{};
        depthDescriptor.type        = clv::gfx::GraphicsImage::Type::_2D;
        depthDescriptor.usageFlags  = clv::gfx::GraphicsImage::UsageMode::DepthStencilAttachment;
        depthDescriptor.dimensions  = { swapchain->getExtent().x, swapchain->getExtent().y };
        depthDescriptor.format      = clv::gfx::ImageFormat::D32_SFLOAT;
        depthDescriptor.sharingMode = clv::gfx::SharingMode::Exclusive;
        depthDescriptor.memoryType  = clv::gfx::MemoryType::VideoMemory;

        depthImage     = graphicsFactory->createImage(std::move(depthDescriptor));
        depthImageView = depthImage->createView();
    }

    void ForwardRenderer3D::createPipeline() {
        clv::gfx::PushConstantDescriptor modelPushConstant{};
        modelPushConstant.stage = clv::gfx::ShaderStage::Vertex;
        modelPushConstant.size  = sizeof(VertexData);

        clv::gfx::PipelineObject::Descriptor pipelineDescriptor;
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
            clv::gfx::Framebuffer::Descriptor frameBufferDescriptor{};
            frameBufferDescriptor.renderPass  = renderPass;
            frameBufferDescriptor.attachments = { swapChainImageView, depthImageView };
            frameBufferDescriptor.width       = swapchain->getExtent().x;
            frameBufferDescriptor.height      = swapchain->getExtent().y;

            swapChainFrameBuffers.emplace_back(graphicsFactory->createFramebuffer(frameBufferDescriptor));
        }
    }

    std::vector<std::shared_ptr<clv::gfx::GraphicsBuffer>> ForwardRenderer3D::createUniformBuffers(const uint32_t bufferCount) {
        std::vector<std::shared_ptr<clv::gfx::GraphicsBuffer>> uniformBuffers(bufferCount);

        const size_t minOffUniformBufferAlignment = graphicsDevice->getLimits().minUniformBufferOffsetAlignment;
        const size_t padding                      = minOffUniformBufferAlignment * 2;//Add padding so that each element has room to be aligned

        for(size_t i = 0; i < bufferCount; ++i) {
            clv::gfx::GraphicsBuffer::Descriptor descriptor{};
            descriptor.size        = uniformBufferLayout.totalSize() + padding;
            descriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::UniformBuffer;
            descriptor.sharingMode = clv::gfx::SharingMode::Exclusive;
            descriptor.memoryType  = clv::gfx::MemoryType::SystemMemory;

            uniformBuffers[i] = graphicsFactory->createBuffer(std::move(descriptor));
        }

        return uniformBuffers;
    }

    std::shared_ptr<clv::gfx::DescriptorPool> ForwardRenderer3D::createDescriptorPool(const std::unordered_map<clv::gfx::DescriptorType, uint32_t>& bindingCount, const uint32_t setCount) {
        std::vector<clv::gfx::DescriptorInfo> poolTypes;
        for(auto&& [type, count] : bindingCount) {
            clv::gfx::DescriptorInfo info{};
            info.type  = type;
            info.count = count;

            poolTypes.emplace_back(info);
        }

        clv::gfx::DescriptorPool::Descriptor poolDescriptor{};
        poolDescriptor.poolTypes = std::move(poolTypes);
        poolDescriptor.flag      = clv::gfx::DescriptorPool::Flag::None;
        poolDescriptor.maxSets   = setCount;

        return graphicsFactory->createDescriptorPool(std::move(poolDescriptor));
    }
}