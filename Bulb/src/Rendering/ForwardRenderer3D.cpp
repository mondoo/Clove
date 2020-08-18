#include "Bulb/Rendering/ForwardRenderer3D.hpp"

#include "Bulb/TextureLoader.hpp"
#include "Bulb/Rendering/Material.hpp"
#include "Bulb/Rendering/Vertex.hpp"
#include "Bulb/Rendering/Renderables/Mesh.hpp"

#include <Clove/Graphics/DescriptorSet.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>
#include <Clove/Platform/Window.hpp>
#include <Clove/Graphics/Graphics.hpp>

namespace blb::rnd {
    ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API api) {
        windowResizeHandle = window.onWindowResize.bind(&ForwardRenderer3D::onWindowResize, this);
        windowSize         = window.getSize();

        graphicsFactory = clv::gfx::createGraphicsFactory(api, window.getNativeWindow());

        graphicsQueue = graphicsFactory->createGraphicsQueue({ clv::gfx::QueueFlags::ReuseBuffers });
        presentQueue  = graphicsFactory->createPresentQueue();
        transferQueue = graphicsFactory->createTransferQueue({ clv::gfx::QueueFlags::Transient });

        swapchain = graphicsFactory->createSwapChain({ windowSize });
        createRenderpass();

        createDepthBuffer();

        //TODO: Retrieve these from the shaders? Can these be created if the shader doesn't want them?
        clv::gfx::DescriptorBindingInfo uboLayoutBinding{};
        uboLayoutBinding.binding   = 0;
        uboLayoutBinding.type      = clv::gfx::DescriptorType::UniformBuffer;
        uboLayoutBinding.arraySize = 1;
        uboLayoutBinding.stage     = clv::gfx::DescriptorStage::Vertex;

        clv::gfx::DescriptorBindingInfo samplerLayoutBinding{};
        samplerLayoutBinding.binding   = 1;
        samplerLayoutBinding.type      = clv::gfx::DescriptorType::CombinedImageSampler;
        samplerLayoutBinding.arraySize = 1;
        samplerLayoutBinding.stage     = clv::gfx::DescriptorStage::Pixel;

        clv::gfx::DescriptorSetLayout::Descriptor descriptorSetLayoutDescriptor{};
        descriptorSetLayoutDescriptor.bindings = { uboLayoutBinding, samplerLayoutBinding };

        descriptorSetLayout = graphicsFactory->createDescriptorSetLayout(descriptorSetLayoutDescriptor);

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
        graphicsFactory->waitForIdleDevice();

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
    }

    void ForwardRenderer3D::submitStaticMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform) {
        currentFrameData.meshes.push_back({ std::move(mesh), std::move(transform) });
    }

    void ForwardRenderer3D::submitAnimatedMesh(std::shared_ptr<rnd::Mesh> mesh, clv::mth::mat4f transform) {
    }

    void ForwardRenderer3D::submitLight(const DirectionalLight& light) {
    }

    void ForwardRenderer3D::submitLight(const PointLight& light) {
    }

    void ForwardRenderer3D::submitCamera(const ComposedCameraData& camera) {
    }

    void ForwardRenderer3D::submitWidget(const std::shared_ptr<Sprite>& widget) {
    }

    void ForwardRenderer3D::submitText(const std::shared_ptr<Sprite>& text) {
    }

    void ForwardRenderer3D::end() {
        if(needNewSwapchain) {
            recreateSwapchain();
        }

        //Wait on the current frame / current images to be available
        inFlightFences[currentFrame]->wait();

        //Aquire the next available image
        clv::gfx::Result result = swapchain->aquireNextImage(imageAvailableSemaphores[currentFrame].get(), imageIndex);
        if(result == clv::gfx::Result::Error_SwapchainOutOfDate) {
            recreateSwapchain();
        }

        //Check if we're already using this image, if so wait
        if(imagesInFlight[imageIndex] != nullptr) {
            imagesInFlight[imageIndex]->wait();
        }
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];//Ptr copy here, could be slowing things down

        inFlightFences[currentFrame]->reset();

        //Allocate a descriptor set for each mesh to be drawn
        const size_t meshCount = std::size(currentFrameData.meshes);
        if(maxDescriptorSets < meshCount) {
            maxDescriptorSets = meshCount;
            descriptorPool    = createDescriptorPool(maxDescriptorSets * swapChainFrameBuffers.size()); //Make sure to pool enough for all images
        }

        const size_t numDescriptorSets = std::size(descriptorSets[imageIndex]);
        if(numDescriptorSets < meshCount) {
            if(numDescriptorSets > 0) {
                descriptorPool->freeDescriptorSets(descriptorSets[imageIndex]);
            }

            std::vector<std::shared_ptr<clv::gfx::DescriptorSetLayout>> layouts(meshCount, descriptorSetLayout);
            descriptorSets[imageIndex] = descriptorPool->allocateDescriptorSets(layouts);
        }

        if(std::size(uniformBuffers[imageIndex]) < meshCount) {
            uniformBuffers[imageIndex] = createUniformBuffers(meshCount);
        }

        //Record our command buffers
        clv::gfx::RenderArea renderArea{};
        renderArea.origin = { 0, 0 };
        renderArea.size   = { swapchain->getExtent().x, swapchain->getExtent().y };

        clv::mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };
        clv::gfx::DepthStencilValue depthStencilClearValue{ 1.0f, 0 };

        commandBuffers[imageIndex]->beginRecording(clv::gfx::CommandBufferUsage::Default);
        commandBuffers[imageIndex]->beginRenderPass(*renderPass, *swapChainFrameBuffers[imageIndex], renderArea, clearColour, depthStencilClearValue);
        commandBuffers[imageIndex]->bindPipelineObject(*pipelineObject);

        size_t meshIndex = 0;
        for(auto&& [mesh, transform] : currentFrameData.meshes) {
            ModelViewProj ubo{};
            ubo.model = transform;
            ubo.view  = glm::lookAt(glm::vec3(0.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            ubo.proj  = glm::perspective(glm::radians(45.0f), static_cast<float>(swapchain->getExtent().x) / static_cast<float>(swapchain->getExtent().y), 0.1f, 10.0f);

            uniformBuffers[imageIndex][meshIndex]->map(&ubo, sizeof(ubo));

            descriptorSets[imageIndex][meshIndex]->write(*uniformBuffers[imageIndex][meshIndex], 0, sizeof(ModelViewProj), 0);
            descriptorSets[imageIndex][meshIndex]->write(*mesh->getMaterial().diffuseView, *sampler, clv::gfx::ImageLayout::ShaderReadOnlyOptimal, 1);

            commandBuffers[imageIndex]->bindVertexBuffer(*mesh->getVertexBuffer(), 0);
            commandBuffers[imageIndex]->bindIndexBuffer(*mesh->getIndexBuffer(), clv::gfx::IndexType::Uint16);
            commandBuffers[imageIndex]->bindDescriptorSet(*descriptorSets[imageIndex][meshIndex], *pipelineObject);
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
        result = presentQueue->present(presentInfo);
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

        graphicsFactory->waitForIdleDevice();

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
        
        uniformBuffers.resize(imageCount);
        descriptorSets.resize(imageCount);

        commandBuffers.reserve(imageCount);
        for(size_t i = 0; i < imageCount; ++i) {
            commandBuffers.emplace_back(graphicsQueue->allocateCommandBuffer());
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
        clv::gfx::SubpassDependency dependecy{};
        dependecy.sourceSubpass      = clv::gfx::SUBPASS_EXTERNAL;
        dependecy.destinationSubpass = 0;
        dependecy.sourceStage        = clv::gfx::PipelineStage::ColourAttachmentOutput;
        dependecy.destinationStage   = clv::gfx::PipelineStage::ColourAttachmentOutput;
        dependecy.sourceAccess       = clv::gfx::AccessFlags::None;
        dependecy.destinationAccess  = clv::gfx::AccessFlags::ColourAttachmentWrite;

        //Create render pass
        clv::gfx::RenderPass::Descriptor renderPassDescriptor{};
        renderPassDescriptor.attachments  = { std::move(colourAttachment), std::move(depthAttachment) };
        renderPassDescriptor.subpasses    = { std::move(subpass) };
        renderPassDescriptor.dependencies = { std::move(dependecy) };

        renderPass = graphicsFactory->createRenderPass(std::move(renderPassDescriptor));
    }

    void ForwardRenderer3D::createDepthBuffer() {
        clv::gfx::GraphicsImage::Descriptor depthDescriptor{};
        depthDescriptor.type        = clv::gfx::GraphicsImage::Type::_2D;
        depthDescriptor.usageFlags  = clv::gfx::GraphicsImage::UsageMode::DepthStencilAttachment;
        depthDescriptor.dimensions  = { swapchain->getExtent().x, swapchain->getExtent().y };
        depthDescriptor.format      = clv::gfx::ImageFormat::D32_SFLOAT;
        depthDescriptor.sharingMode = clv::gfx::SharingMode::Concurrent;
        depthDescriptor.memoryType  = clv::gfx::MemoryType::VideoMemory;

        depthImage     = graphicsFactory->createImage(std::move(depthDescriptor));
        depthImageView = depthImage->createView();
    }

    void ForwardRenderer3D::createPipeline() {
        clv::gfx::PipelineObject::Descriptor pipelineDescriptor;
        pipelineDescriptor.vertexShader            = graphicsFactory->createShader("vert.spirv");
        pipelineDescriptor.fragmentShader          = graphicsFactory->createShader("frag.spirv");
        pipelineDescriptor.vertexInput             = Vertex::getInputBindingDescriptor();
        pipelineDescriptor.vertexAttributes        = Vertex::getVertexAttributes();
        pipelineDescriptor.viewportDescriptor.size = { swapchain->getExtent().x, swapchain->getExtent().y };
        pipelineDescriptor.scissorDescriptor.size  = { swapchain->getExtent().x, swapchain->getExtent().y };
        pipelineDescriptor.renderPass              = renderPass;
        pipelineDescriptor.descriptorSetLayouts    = { descriptorSetLayout };

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

        for(size_t i = 0; i < bufferCount; ++i) {
            clv::gfx::GraphicsBuffer::Descriptor descriptor{};
            descriptor.size        = sizeof(ModelViewProj);
            descriptor.usageFlags  = clv::gfx::GraphicsBuffer::UsageMode::UniformBuffer;
            descriptor.sharingMode = clv::gfx::SharingMode::Exclusive;
            descriptor.memoryType  = clv::gfx::MemoryType::SystemMemory;

            uniformBuffers[i] = graphicsFactory->createBuffer(std::move(descriptor));
        }

        return uniformBuffers;
    }

    std::shared_ptr<clv::gfx::DescriptorPool> ForwardRenderer3D::createDescriptorPool(const uint32_t setCount) {
        clv::gfx::DescriptorInfo uboInfo{};
        uboInfo.type  = clv::gfx::DescriptorType::UniformBuffer;
        uboInfo.count = setCount;

        clv::gfx::DescriptorInfo samplerInfo{};
        samplerInfo.type  = clv::gfx::DescriptorType::CombinedImageSampler;
        samplerInfo.count = setCount;

        clv::gfx::DescriptorPool::Descriptor poolDescriptor{};
        poolDescriptor.poolTypes = { std::move(uboInfo), std::move(samplerInfo) };
        poolDescriptor.flag      = clv::gfx::DescriptorPool::Flag::None;
        poolDescriptor.maxSets   = setCount;

        return graphicsFactory->createDescriptorPool(std::move(poolDescriptor));
    }
}