#include "Clove/Graphics/Vulkan/VKGraphicsFactory.hpp"

#include "Clove/Graphics/ShaderCompiler.hpp"
#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorPool.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsQueue.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKPresentQueue.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKSampler.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKShader.hpp"
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"
#include "Clove/Graphics/Vulkan/VKTransferQueue.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Log/Log.hpp>
#include <fstream>

namespace garlic::clove {
    namespace {
        VkCommandPoolCreateFlags convertCommandPoolCreateFlags(QueueFlags garlicFlags) {
            switch(garlicFlags) {
                case QueueFlags::None:
                    return 0;
                case QueueFlags::Transient:
                    return VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
                case QueueFlags::ReuseBuffers:
                    return VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown queue flag", CLOVE_FUNCTION_NAME);
                    return 0;
            }
        }

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const &availableFormats) {
            for(auto const &availableFormat : availableFormats) {
                if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return availableFormat;
                }
            }

            //Fall back to the first one if we can't find a surface format we want
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Warning, "Swapchain could not find desired format. Using first available format from the surface");
            return availableFormats[0];
        }

        VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> const &availablePresentModes) {
            for(auto const &availablePresentMode : availablePresentModes) {
                if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return availablePresentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR const &capabilities, VkExtent2D const &windowExtent) {
            if(capabilities.currentExtent.width != UINT32_MAX) {//If width / height are max then the window manager is allowing us to differ in size
                return capabilities.currentExtent;
            } else {
                return VkExtent2D{
                    .width  = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, windowExtent.width)),
                    .height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, windowExtent.height)),
                };
            }
        }

        VkAttachmentLoadOp convertLoadOp(LoadOperation garlicOperation) {
            switch(garlicOperation) {
                case LoadOperation::DontCare:
                    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                case LoadOperation::Clear:
                    return VK_ATTACHMENT_LOAD_OP_CLEAR;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown operation", CLOVE_FUNCTION_NAME);
                    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            }
        }

        VkAttachmentStoreOp convertStoreOp(StoreOperation garlicOperation) {
            switch(garlicOperation) {
                case StoreOperation::DontCare:
                    return VK_ATTACHMENT_STORE_OP_DONT_CARE;
                case StoreOperation::Store:
                    return VK_ATTACHMENT_STORE_OP_STORE;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown operation", CLOVE_FUNCTION_NAME);
                    return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            }
        }

        VkDescriptorType getDescriptorType(DescriptorType garlicType) {
            switch(garlicType) {
                case DescriptorType::UniformBuffer:
                    return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                case DescriptorType::CombinedImageSampler:
                    return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown type", CLOVE_FUNCTION_NAME);
                    return VK_DESCRIPTOR_TYPE_MAX_ENUM;
            }
        }

        VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat) {
            switch(garlicFormat) {
                case VertexAttributeFormat::R32_SFLOAT:
                    return VK_FORMAT_R32_SFLOAT;
                case VertexAttributeFormat::R32G32_SFLOAT:
                    return VK_FORMAT_R32G32_SFLOAT;
                case VertexAttributeFormat::R32G32B32_SFLOAT:
                    return VK_FORMAT_R32G32B32_SFLOAT;
                case VertexAttributeFormat::R32G32B32A32_SFLOAT:
                    return VK_FORMAT_R32G32B32A32_SFLOAT;
                case VertexAttributeFormat::R32G32B32A32_SINT:
                    return VK_FORMAT_R32G32B32A32_SINT;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown format passed", CLOVE_FUNCTION_NAME_PRETTY);
                    return VK_FORMAT_UNDEFINED;
            }
        }

        VkDescriptorPoolCreateFlags getDescriptorPoolFlags(DescriptorPool::Flag garlicFlag) {
            switch(garlicFlag) {
                case DescriptorPool::Flag::None:
                    return 0;
                case DescriptorPool::Flag::FreeDescriptorSet:
                    return VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
                default:
                    CLOVE_ASSERT(false, "{0} Unknown flag", CLOVE_FUNCTION_NAME);
            }
        }

        VkBufferUsageFlags getUsageFlags(GraphicsBuffer::UsageMode garlicUsageFlags) {
            VkBufferUsageFlags flags = 0;

            if((garlicUsageFlags & GraphicsBuffer::UsageMode::TransferSource) != 0) {
                flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            }
            if((garlicUsageFlags & GraphicsBuffer::UsageMode::TransferDestination) != 0) {
                flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            }
            if((garlicUsageFlags & GraphicsBuffer::UsageMode::VertexBuffer) != 0) {
                flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            }
            if((garlicUsageFlags & GraphicsBuffer::UsageMode::IndexBuffer) != 0) {
                flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            }
            if((garlicUsageFlags & GraphicsBuffer::UsageMode::UniformBuffer) != 0) {
                flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            }

            return flags;
        }

        VkImageUsageFlags getUsageFlags(GraphicsImage::UsageMode garlicUsageFlags) {
            VkImageUsageFlags flags = 0;

            if((garlicUsageFlags & GraphicsImage::UsageMode::TransferSource) != 0) {
                flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            }
            if((garlicUsageFlags & GraphicsImage::UsageMode::TransferDestination) != 0) {
                flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            }
            if((garlicUsageFlags & GraphicsImage::UsageMode::Sampled) != 0) {
                flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
            }
            if((garlicUsageFlags & GraphicsImage::UsageMode::ColourAttachment) != 0) {
                flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            }
            if((garlicUsageFlags & GraphicsImage::UsageMode::DepthStencilAttachment) != 0) {
                flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            }

            return flags;
        }

        VkImageType getImageType(GraphicsImage::Type garlicImageType) {
            switch(garlicImageType) {
                case GraphicsImage::Type::_2D:
                case GraphicsImage::Type::Cube:
                    return VK_IMAGE_TYPE_2D;
                case GraphicsImage::Type::_3D:
                    return VK_IMAGE_TYPE_3D;
                default:
                    CLOVE_ASSERT(false, "{0}: Unhandled image type");
                    return VK_IMAGE_TYPE_2D;
            }
        }

        VkFilter getFilter(Sampler::Filter garlicfilter) {
            switch(garlicfilter) {
                case Sampler::Filter::Nearest:
                    return VK_FILTER_NEAREST;
                case Sampler::Filter::Linear:
                    return VK_FILTER_LINEAR;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown type", CLOVE_FUNCTION_NAME);
                    return VK_FILTER_NEAREST;
            }
        }

        VkSamplerAddressMode getAddressMode(Sampler::AddressMode garlicMode) {
            switch(garlicMode) {
                case Sampler::AddressMode::Repeat:
                    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
                case Sampler::AddressMode::MirroredRepeat:
                    return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
                case Sampler::AddressMode::ClampToEdge:
                    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
                case Sampler::AddressMode::ClampToBorder:
                    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown type", CLOVE_FUNCTION_NAME);
                    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            }
        }
    }

    VKGraphicsFactory::VKGraphicsFactory(DevicePointer devicePtr, QueueFamilyIndices queueFamilyIndices)
        : devicePtr{ std::move(devicePtr) }
        , queueFamilyIndices{ std::move(queueFamilyIndices) } {
        memoryAllocator = std::make_shared<MemoryAllocator>(this->devicePtr);
    }

    VKGraphicsFactory::VKGraphicsFactory(VKGraphicsFactory &&other) noexcept = default;

    VKGraphicsFactory &VKGraphicsFactory::operator=(VKGraphicsFactory &&other) noexcept = default;

    VKGraphicsFactory::~VKGraphicsFactory() = default;

    Expected<std::unique_ptr<GraphicsQueue>, std::runtime_error> VKGraphicsFactory::createGraphicsQueue(CommandQueueDescriptor descriptor) {
        uint32_t const familyIndex{ *queueFamilyIndices.graphicsFamily };

        VkCommandPoolCreateInfo poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = convertCommandPoolCreateFlags(descriptor.flags),
            .queueFamilyIndex = familyIndex,
        };

        VkCommandPool commandPool;
        if(VkResult const result{ vkCreateCommandPool(devicePtr.get(), &poolInfo, nullptr, &commandPool) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsQueue. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsQueue. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsQueue. Reason unkown." } };
            }
        }

        VkQueue queue;
        vkGetDeviceQueue(devicePtr.get(), familyIndex, 0, &queue);

        return std::unique_ptr<GraphicsQueue>{ std::make_unique<VKGraphicsQueue>(devicePtr, queue, commandPool, queueFamilyIndices) };
    }

    Expected<std::unique_ptr<PresentQueue>, std::runtime_error> VKGraphicsFactory::createPresentQueue() {
        if(!queueFamilyIndices.presentFamily.has_value()) {
            return Unexpected{ std::runtime_error{ "Presentation queue not available. GraphicsDevice is likely headless" } };
        }

        VkQueue queue;
        vkGetDeviceQueue(devicePtr.get(), *queueFamilyIndices.presentFamily, 0, &queue);

        return std::unique_ptr<PresentQueue>{ std::make_unique<VKPresentQueue>(devicePtr, queue) };
    }

    Expected<std::unique_ptr<TransferQueue>, std::runtime_error> VKGraphicsFactory::createTransferQueue(CommandQueueDescriptor descriptor) {
        uint32_t const familyIndex{ *queueFamilyIndices.transferFamily };

        VkCommandPoolCreateInfo const poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = convertCommandPoolCreateFlags(descriptor.flags),
            .queueFamilyIndex = familyIndex,
        };

        VkCommandPool commandPool;
        if(VkResult const result{ vkCreateCommandPool(devicePtr.get(), &poolInfo, nullptr, &commandPool) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create TransferQueue. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create TransferQueue. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create TransferQueue. Reason unkown." } };
            }
        }

        VkQueue queue;
        vkGetDeviceQueue(devicePtr.get(), familyIndex, 0, &queue);

        return std::unique_ptr<TransferQueue>{ std::make_unique<VKTransferQueue>(devicePtr, queue, commandPool, queueFamilyIndices) };
    }

    Expected<std::unique_ptr<Swapchain>, std::runtime_error> VKGraphicsFactory::createSwapChain(Swapchain::Descriptor descriptor) {
        if(devicePtr.getSurface() == VK_NULL_HANDLE) {
            return Unexpected{ std::runtime_error{ "Swapchain is not available. GraphicsDevice is likely headless" } };
        }

        VkExtent2D const windowExtent{ descriptor.extent.x, descriptor.extent.y };

        SurfaceSupportDetails const surfaceSupport{ SurfaceSupportDetails::query(devicePtr.getPhysical(), devicePtr.getSurface()) };

        VkSurfaceFormatKHR const surfaceFormat{ chooseSwapSurfaceFormat(surfaceSupport.formats) };
        VkPresentModeKHR const presentMode{ chooseSwapPresentMode(surfaceSupport.presentModes) };
        VkExtent2D const swapchainExtent{ chooseSwapExtent(surfaceSupport.capabilities, windowExtent) };

        //Request one more than the minimum images the swap chain can support because sometimes we might need to wait for the driver
        uint32_t imageCount = surfaceSupport.capabilities.minImageCount + 1;
        if(surfaceSupport.capabilities.maxImageCount > 0 && imageCount > surfaceSupport.capabilities.maxImageCount) {
            imageCount = surfaceSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR const createInfo{
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface               = devicePtr.getSurface(),
            .minImageCount         = imageCount,
            .imageFormat           = surfaceFormat.format,
            .imageColorSpace       = surfaceFormat.colorSpace,
            .imageExtent           = swapchainExtent,
            .imageArrayLayers      = 1,
            .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices   = nullptr,
            .preTransform          = surfaceSupport.capabilities.currentTransform,
            .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode           = presentMode,
            .clipped               = VK_TRUE,
            .oldSwapchain          = VK_NULL_HANDLE,
        };

        VkSwapchainKHR swapchain;
        if(VkResult const result{ vkCreateSwapchainKHR(devicePtr.get(), &createInfo, nullptr, &swapchain) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Out of device memory" } };
                case VK_ERROR_DEVICE_LOST:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Device lost." } };
                case VK_ERROR_SURFACE_LOST_KHR:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Surface lost." } };
                case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Native window is in use." } };
                case VK_ERROR_INITIALIZATION_FAILED:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Initialisation has failed." } };
                case VK_ERROR_VALIDATION_FAILED_EXT:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Validation has failed." } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create Swapchain. Reason unkown." } };
            }
        }

        return std::unique_ptr<Swapchain>{ std::make_unique<VKSwapchain>(devicePtr, swapchain, surfaceFormat.format, std::move(swapchainExtent)) };
    }

    Expected<std::unique_ptr<Shader>, std::runtime_error> VKGraphicsFactory::createShader(std::string_view filePath, Shader::Stage shaderStage) {
        return createShaderObject(ShaderCompiler::compileFromFile(filePath, shaderStage, ShaderType::SPIRV));
    }

    Expected<std::unique_ptr<Shader>, std::runtime_error> VKGraphicsFactory::createShader(std::span<std::byte const> source, Shader::Stage shaderStage) {
        return createShaderObject(ShaderCompiler::compileFromSource(source, shaderStage, ShaderType::SPIRV));
    }

    Expected<std::unique_ptr<RenderPass>, std::runtime_error> VKGraphicsFactory::createRenderPass(RenderPass::Descriptor descriptor) {
        //Attachments
        const size_t attachmentSize = std::size(descriptor.attachments);
        std::vector<VkAttachmentDescription> attachments(attachmentSize);
        for(size_t i = 0; i < attachmentSize; ++i) {
            attachments[i] = VkAttachmentDescription{
                .format         = VKImage::convertFormat(descriptor.attachments[i].format),
                .samples        = VK_SAMPLE_COUNT_1_BIT,
                .loadOp         = convertLoadOp(descriptor.attachments[i].loadOperation),
                .storeOp        = convertStoreOp(descriptor.attachments[i].storeOperation),
                .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout  = VKImage::convertLayout(descriptor.attachments[i].initialLayout),
                .finalLayout    = VKImage::convertLayout(descriptor.attachments[i].finalLayout),
            };
        }

        //Subpasses
        const size_t subpassesSize = std::size(descriptor.subpasses);
        std::vector<VkSubpassDescription> subpasses(subpassesSize);

        //Define the references seperately so they aren't destroyed
        std::vector<std::vector<VkAttachmentReference>> attachmentReferences(subpassesSize);
        std::vector<std::optional<VkAttachmentReference>> depthStencilAttachment(subpassesSize);

        for(size_t i = 0; i < subpassesSize; ++i) {
            //Attachment References: Colour
            const size_t colourAttachmentSize = std::size(descriptor.subpasses[i].colourAttachments);
            attachmentReferences[i].resize(colourAttachmentSize);
            for(size_t j = 0; j < colourAttachmentSize; ++j) {
                attachmentReferences[i][j] = VkAttachmentReference{
                    .attachment = descriptor.subpasses[i].colourAttachments[j].attachmentIndex,
                    .layout     = VKImage::convertLayout(descriptor.subpasses[i].colourAttachments[j].layout),
                };
            }

            //Attachment References: Depth
            if(descriptor.subpasses[i].depthAttachment) {
                depthStencilAttachment[i] = VkAttachmentReference{
                    .attachment = descriptor.subpasses[i].depthAttachment->attachmentIndex,
                    .layout     = VKImage::convertLayout(descriptor.subpasses[i].depthAttachment->layout),
                };
            }

            subpasses[i] = VkSubpassDescription{
                .flags                   = 0,
                .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,//TODO: Only supporting graphics for now
                .inputAttachmentCount    = 0,
                .pInputAttachments       = nullptr,
                .colorAttachmentCount    = static_cast<uint32_t>(colourAttachmentSize),
                .pColorAttachments       = std::data(attachmentReferences[i]),
                .pResolveAttachments     = nullptr,
                .pDepthStencilAttachment = depthStencilAttachment[i].has_value() ? &depthStencilAttachment[i].value() : nullptr,
                .preserveAttachmentCount = 0,
                .pPreserveAttachments    = nullptr,
            };
        }

        //Dependencies
        const size_t dependecySize = std::size(descriptor.dependencies);
        std::vector<VkSubpassDependency> dependecies(dependecySize);
        for(size_t i = 0; i < dependecySize; ++i) {
            dependecies[i] = VkSubpassDependency{
                .srcSubpass    = descriptor.dependencies[i].sourceSubpass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : descriptor.dependencies[i].sourceSubpass,
                .dstSubpass    = descriptor.dependencies[i].destinationSubpass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : descriptor.dependencies[i].destinationSubpass,
                .srcStageMask  = VKPipelineObject::convertStage(descriptor.dependencies[i].sourceStage),
                .dstStageMask  = VKPipelineObject::convertStage(descriptor.dependencies[i].destinationStage),
                .srcAccessMask = convertAccessFlags(descriptor.dependencies[i].sourceAccess),
                .dstAccessMask = convertAccessFlags(descriptor.dependencies[i].destinationAccess),
            };
        }

        //Renderpass
        VkRenderPassCreateInfo const renderPassInfo{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<uint32_t>(attachmentSize),
            .pAttachments    = std::data(attachments),
            .subpassCount    = static_cast<uint32_t>(subpassesSize),
            .pSubpasses      = std::data(subpasses),
            .dependencyCount = static_cast<uint32_t>(dependecySize),
            .pDependencies   = std::data(dependecies),
        };

        VkRenderPass renderPass;
        if(VkResult const result{ vkCreateRenderPass(devicePtr.get(), &renderPassInfo, nullptr, &renderPass) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create RenderPass. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create RenderPass. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create RenderPass. Reason unkown." } };
            }
        }

        return std::unique_ptr<RenderPass>{ std::make_unique<VKRenderPass>(devicePtr, renderPass) };
    }

    Expected<std::unique_ptr<DescriptorSetLayout>, std::runtime_error> VKGraphicsFactory::createDescriptorSetLayout(DescriptorSetLayout::Descriptor descriptor) {
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings(std::size(descriptor.bindings));
        for(size_t i = 0; i < std::size(layoutBindings); ++i) {
            auto const &bindingDescriptor{ descriptor.bindings[i] };
            layoutBindings[i] = VkDescriptorSetLayoutBinding{
                .binding            = bindingDescriptor.binding,
                .descriptorType     = getDescriptorType(bindingDescriptor.type),
                .descriptorCount    = static_cast<uint32_t>(bindingDescriptor.arraySize),
                .stageFlags         = VKShader::convertStage(bindingDescriptor.stage),
                .pImmutableSamplers = nullptr,
            };
        }

        VkDescriptorSetLayoutCreateInfo const createInfo{
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext        = nullptr,
            .flags        = 0,
            .bindingCount = static_cast<uint32_t>(std::size(layoutBindings)),
            .pBindings    = std::data(layoutBindings),
        };

        VkDescriptorSetLayout layout;
        if(VkResult const result{ vkCreateDescriptorSetLayout(devicePtr.get(), &createInfo, nullptr, &layout) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create DescriptorSetLayout. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create DescriptorSetLayout. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create DescriptorSetLayout. Reason unkown." } };
            }
        }

        return std::unique_ptr<DescriptorSetLayout>{ std::make_unique<VKDescriptorSetLayout>(devicePtr, layout, std::move(descriptor)) };
    }

    Expected<std::unique_ptr<PipelineObject>, std::runtime_error> VKGraphicsFactory::createPipelineObject(PipelineObject::Descriptor descriptor) {
        //Descriptor set layouts
        size_t const descriptorLayoutCount{ std::size(descriptor.descriptorSetLayouts) };
        std::vector<VkDescriptorSetLayout> descriptorLayouts(descriptorLayoutCount);
        for(size_t i = 0; i < descriptorLayoutCount; ++i) {
            descriptorLayouts[i] = polyCast<VKDescriptorSetLayout>(descriptor.descriptorSetLayouts[i].get())->getLayout();
        }

        //Push constants
        std::vector<VkPushConstantRange> vkPushConstantRanges(std::size(descriptor.pushConstants));
        for(size_t i = 0; i < std::size(vkPushConstantRanges); ++i) {
            vkPushConstantRanges[i] = VkPushConstantRange{
                .stageFlags = VKShader::convertStage(descriptor.pushConstants[i].stage),
                .offset     = static_cast<uint32_t>(descriptor.pushConstants[i].offset),
                .size       = static_cast<uint32_t>(descriptor.pushConstants[i].size),
            };
        }

        //Pipeline Layout
        VkPipelineLayoutCreateInfo const pipelineLayoutInfo{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount         = static_cast<uint32_t>(std::size(descriptorLayouts)),
            .pSetLayouts            = std::data(descriptorLayouts),
            .pushConstantRangeCount = static_cast<uint32_t>(std::size(vkPushConstantRanges)),
            .pPushConstantRanges    = std::data(vkPushConstantRanges),
        };

        VkPipelineLayout pipelineLayout;
        if(VkResult const result{ vkCreatePipelineLayout(devicePtr.get(), &pipelineLayoutInfo, nullptr, &pipelineLayout) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create PipelineObject's layout. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create PipelineObject's layout. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create PipelineObject's layout. Reason unkown." } };
            }
        }

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

        //Vertex shader
        shaderStages[0] = VkPipelineShaderStageCreateInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = VK_SHADER_STAGE_VERTEX_BIT,
            .module = polyCast<VKShader>(descriptor.vertexShader.get())->getModule(),
            .pName  = "main",
        };

        //Fragment shader
        shaderStages[1] = VkPipelineShaderStageCreateInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = polyCast<VKShader>(descriptor.fragmentShader.get())->getModule(),
            .pName  = "main",
        };

        //Vertex input
        VkVertexInputBindingDescription const inputBindingDescription{
            .binding   = 0,
            .stride    = descriptor.vertexInput.stride,
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        size_t const vertexAttributeCount{ std::size(descriptor.vertexAttributes) };
        std::vector<VkVertexInputAttributeDescription> vertexAttributes(vertexAttributeCount);
        for(size_t i = 0; i < vertexAttributeCount; ++i) {
            auto const &attribute = descriptor.vertexAttributes[i];
            vertexAttributes[i]   = VkVertexInputAttributeDescription{
                .location = attribute.location,
                .binding  = 0,
                .format   = convertAttributeFormat(attribute.format),
                .offset   = attribute.offset,
            };
        }

        VkPipelineVertexInputStateCreateInfo const vertexInputInfo{
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount   = 1,
            .pVertexBindingDescriptions      = &inputBindingDescription,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(std::size(vertexAttributes)),
            .pVertexAttributeDescriptions    = std::data(vertexAttributes),
        };

        //Input assembly
        VkPipelineInputAssemblyStateCreateInfo const inputAssembly{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
        };

        //Viewport + Scissor
        //TODO: Dynamic viewport / scissor rect
        VkViewport const viewport{
            .x        = static_cast<float>(descriptor.viewportDescriptor.position.x),
            .y        = static_cast<float>(descriptor.viewportDescriptor.position.y),
            .width    = static_cast<float>(descriptor.viewportDescriptor.size.x),
            .height   = static_cast<float>(descriptor.viewportDescriptor.size.y),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D const scissor{
            .offset = { descriptor.scissorDescriptor.position.x, descriptor.scissorDescriptor.position.y },
            .extent = { descriptor.scissorDescriptor.size.x, descriptor.scissorDescriptor.size.y },
        };

        VkPipelineViewportStateCreateInfo const viewportState{
            .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports    = &viewport,
            .scissorCount  = 1,
            .pScissors     = &scissor,
        };

        //Rasteriser
        VkPipelineRasterizationStateCreateInfo const rasterizer{
            .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable        = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode             = VK_POLYGON_MODE_FILL,
            .cullMode                = VK_CULL_MODE_BACK_BIT,
            .frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable         = VK_FALSE,
            .lineWidth               = 1.0f,//Any thicker requires the 'wideLines' feature
        };

        //Multisampling
        VkPipelineMultisampleStateCreateInfo const multisampling{
            .sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable  = VK_FALSE,
        };

        //Depth / Stencil
        VkPipelineDepthStencilStateCreateInfo const depthStencil{
            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .depthTestEnable       = static_cast<VkBool32>(descriptor.depthState.depthTest ? VK_TRUE : VK_FALSE),
            .depthWriteEnable      = static_cast<VkBool32>(descriptor.depthState.depthWrite ? VK_TRUE : VK_FALSE),
            .depthCompareOp        = VK_COMPARE_OP_LESS,
            .depthBoundsTestEnable = VK_FALSE,
            .stencilTestEnable     = VK_FALSE,
            .front                 = {},
            .back                  = {},
            .minDepthBounds        = 0.0f,
            .maxDepthBounds        = 1.0f,
        };

        //Colour blending
        VkPipelineColorBlendAttachmentState const colourBlendAttachment{
            .blendEnable         = static_cast<VkBool32>(descriptor.enableBlending ? VK_TRUE : VK_FALSE),
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp        = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp        = VK_BLEND_OP_ADD,
            .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };

        VkPipelineColorBlendStateCreateInfo const colorBlending{
            .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable   = VK_FALSE,
            .logicOp         = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments    = &colourBlendAttachment,
            .blendConstants  = { 0.0f, 0.0f, 0.0f, 0.0f },
        };

        //Pipeline
        VkGraphicsPipelineCreateInfo const pipelineInfo{
            .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext               = nullptr,
            .stageCount          = std::size(shaderStages),
            .pStages             = std::data(shaderStages),
            .pVertexInputState   = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pTessellationState  = nullptr,
            .pViewportState      = &viewportState,
            .pRasterizationState = &rasterizer,
            .pMultisampleState   = &multisampling,
            .pDepthStencilState  = &depthStencil,
            .pColorBlendState    = &colorBlending,
            .pDynamicState       = nullptr,//TODO
            .layout              = pipelineLayout,
            .renderPass          = polyCast<VKRenderPass>(descriptor.renderPass.get())->getRenderPass(),
            .subpass             = 0,//The subpass of the renderpass that'll use this pipeline
            .basePipelineHandle  = VK_NULL_HANDLE,
            .basePipelineIndex   = -1,
        };

        VkPipeline pipeline;
        if(VkResult const result{ vkCreateGraphicsPipelines(devicePtr.get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create PipelineObject. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create PipelineObject. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create PipelineObject. Reason unkown." } };
            }
        }

        return std::unique_ptr<PipelineObject>{ std::make_unique<VKPipelineObject>(devicePtr, pipeline, pipelineLayout) };
    }

    Expected<std::unique_ptr<Framebuffer>, std::runtime_error> VKGraphicsFactory::createFramebuffer(Framebuffer::Descriptor descriptor) {
        std::vector<VkImageView> attachments;
        attachments.reserve(std::size(descriptor.attachments));
        for(auto &attachment : descriptor.attachments) {
            attachments.push_back(polyCast<VKImageView>(attachment.get())->getImageView());
        }

        VkFramebufferCreateInfo const framebufferInfo{
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .renderPass      = polyCast<VKRenderPass>(descriptor.renderPass.get())->getRenderPass(),
            .attachmentCount = static_cast<uint32_t>(std::size(attachments)),
            .pAttachments    = std::data(attachments),
            .width           = descriptor.width,
            .height          = descriptor.height,
            .layers          = 1,
        };

        VkFramebuffer framebuffer;
        if(VkResult const result{ vkCreateFramebuffer(devicePtr.get(), &framebufferInfo, nullptr, &framebuffer) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Framebuffer. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Framebuffer. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create Framebuffer. Reason unkown." } };
            }
        }

        return std::unique_ptr<Framebuffer>{ std::make_unique<VKFramebuffer>(devicePtr, framebuffer) };
    }

    Expected<std::unique_ptr<DescriptorPool>, std::runtime_error> VKGraphicsFactory::createDescriptorPool(DescriptorPool::Descriptor descriptor) {
        size_t const numDescriptorTypes = std::size(descriptor.poolTypes);
        std::vector<VkDescriptorPoolSize> poolSizes(numDescriptorTypes);
        for(size_t i = 0; i < numDescriptorTypes; ++i) {
            poolSizes[i] = VkDescriptorPoolSize{
                .type            = getDescriptorType(descriptor.poolTypes[i].type),
                .descriptorCount = descriptor.poolTypes[i].count,
            };
        }

        VkDescriptorPoolCreateInfo const createInfo{
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext         = nullptr,
            .flags         = getDescriptorPoolFlags(descriptor.flag),
            .maxSets       = descriptor.maxSets,
            .poolSizeCount = static_cast<uint32_t>(std::size(poolSizes)),
            .pPoolSizes    = std::data(poolSizes),
        };

        VkDescriptorPool pool;
        if(VkResult const result{ vkCreateDescriptorPool(devicePtr.get(), &createInfo, nullptr, &pool) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create DescriptorPool. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create DescriptorPool. Out of device memory" } };
                case VK_ERROR_FRAGMENTATION_EXT:
                    return Unexpected{ std::runtime_error{ "Failed to create DescriptorPool. Memory is too fragmented" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create DescriptorPool. Reason unkown." } };
            }
        }

        return std::unique_ptr<DescriptorPool>{ std::make_unique<VKDescriptorPool>(devicePtr, pool, std::move(descriptor)) };
    }

    Expected<std::unique_ptr<Semaphore>, std::runtime_error> VKGraphicsFactory::createSemaphore() {
        VkSemaphoreCreateInfo const createInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        };

        VkSemaphore semaphore;
        if(VkResult const result{ vkCreateSemaphore(devicePtr.get(), &createInfo, nullptr, &semaphore) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Semaphore. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Semaphore. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create Semaphore. Reason unkown." } };
            }
        }

        return std::unique_ptr<Semaphore>{ std::make_unique<VKSemaphore>(devicePtr, semaphore) };
    }

    Expected<std::unique_ptr<Fence>, std::runtime_error> VKGraphicsFactory::createFence(Fence::Descriptor descriptor) {
        VkFenceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = descriptor.signaled ? VK_FENCE_CREATE_SIGNALED_BIT : static_cast<VkFenceCreateFlags>(0),
        };

        VkFence fence;
        if(VkResult const result{ vkCreateFence(devicePtr.get(), &createInfo, nullptr, &fence) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Fence. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Fence. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create Fence. Reason unkown." } };
            }
        }

        return std::unique_ptr<Fence>{ std::make_unique<VKFence>(devicePtr, fence) };
    }

    Expected<std::unique_ptr<GraphicsBuffer>, std::runtime_error> VKGraphicsFactory::createBuffer(GraphicsBuffer::Descriptor descriptor) {
        std::array const sharedQueueIndices{ *queueFamilyIndices.graphicsFamily, *queueFamilyIndices.transferFamily };
        bool const isExclusive{ descriptor.sharingMode == SharingMode::Exclusive };

        VkBufferCreateInfo const createInfo{
            .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .size                  = descriptor.size,
            .usage                 = getUsageFlags(descriptor.usageFlags),
            .sharingMode           = isExclusive ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = isExclusive ? 0 : static_cast<uint32_t>(std::size(sharedQueueIndices)),
            .pQueueFamilyIndices   = isExclusive ? nullptr : std::data(sharedQueueIndices),
        };

        VkBuffer buffer;
        if(VkResult const result{ vkCreateBuffer(devicePtr.get(), &createInfo, nullptr, &buffer) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsBuffer. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsBuffer. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsBuffer. Reason unkown." } };
            }
        }

        return std::unique_ptr<GraphicsBuffer>{ std::make_unique<VKBuffer>(devicePtr, buffer, std::move(descriptor), memoryAllocator) };
    }

    Expected<std::unique_ptr<GraphicsImage>, std::runtime_error> VKGraphicsFactory::createImage(GraphicsImage::Descriptor descriptor) {
        std::array const sharedQueueIndices{ *queueFamilyIndices.graphicsFamily, *queueFamilyIndices.transferFamily };
        bool const isExclusive{ descriptor.sharingMode == SharingMode::Exclusive };
        bool const isCube{ descriptor.type == GraphicsImage::Type::Cube };

        VkImageCreateInfo const createInfo{
            .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = isCube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u,
            .imageType             = getImageType(descriptor.type),
            .format                = VKImage::convertFormat(descriptor.format),
            .extent                = { descriptor.dimensions.x, descriptor.dimensions.y, 1 },
            .mipLevels             = 1,
            .arrayLayers           = isCube ? 6u : 1u,
            .samples               = VK_SAMPLE_COUNT_1_BIT,
            .tiling                = VK_IMAGE_TILING_OPTIMAL,
            .usage                 = getUsageFlags(descriptor.usageFlags),
            .sharingMode           = isExclusive ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = isExclusive ? 0 : static_cast<uint32_t>(std::size(sharedQueueIndices)),
            .pQueueFamilyIndices   = isExclusive ? nullptr : std::data(sharedQueueIndices),
            .initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        VkImage image;
        if(VkResult const result{ vkCreateImage(devicePtr.get(), &createInfo, nullptr, &image) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsImage. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsImage. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create GraphicsImage. Reason unkown." } };
            }
        }

        return std::unique_ptr<GraphicsImage>{ std::make_unique<VKImage>(devicePtr, image, std::move(descriptor), memoryAllocator) };
    }

    Expected<std::unique_ptr<Sampler>, std::runtime_error> VKGraphicsFactory::createSampler(Sampler::Descriptor descriptor) {
        VkSamplerCreateInfo const createInfo{
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .magFilter               = getFilter(descriptor.magFilter),
            .minFilter               = getFilter(descriptor.minFilter),
            .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU            = getAddressMode(descriptor.addressModeU),
            .addressModeV            = getAddressMode(descriptor.addressModeV),
            .addressModeW            = getAddressMode(descriptor.addressModeW),
            .mipLodBias              = 0.0f,
            .anisotropyEnable        = static_cast<VkBool32>(descriptor.enableAnisotropy ? VK_TRUE : VK_FALSE),
            .maxAnisotropy           = descriptor.maxAnisotropy,
            .compareEnable           = VK_FALSE,
            .compareOp               = VK_COMPARE_OP_ALWAYS,
            .minLod                  = 0.0f,
            .maxLod                  = 0.0f,
            .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE,
        };

        VkSampler sampler;
        if(VkResult const result{ vkCreateSampler(devicePtr.get(), &createInfo, nullptr, &sampler) }; result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Sampler. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Sampler. Out of device memory" } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create Sampler. Reason unkown." } };
            }
        }

        return std::unique_ptr<Sampler>{ std::make_unique<VKSampler>(devicePtr, sampler) };
    }

    Expected<std::unique_ptr<Shader>, std::runtime_error> VKGraphicsFactory::createShaderObject(std::span<std::byte const> spirvSource) {
        VkShaderModuleCreateInfo const createInfo{
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext    = nullptr,
            .flags    = 0,
            .codeSize = spirvSource.size_bytes(),
            .pCode    = reinterpret_cast<uint32_t const *>(std::data(spirvSource)),
        };

        VkShaderModule module;
        if(VkResult const result = vkCreateShaderModule(devicePtr.get(), &createInfo, nullptr, &module); result != VK_SUCCESS) {
            switch(result) {
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Shader. Out of host memory" } };
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    return Unexpected{ std::runtime_error{ "Failed to create Shader. Out of device memory" } };
                case VK_ERROR_INVALID_SHADER_NV:
                    return Unexpected{ std::runtime_error{ "Failed to create Shader. Shader failed to compile." } };
                default:
                    return Unexpected{ std::runtime_error{ "Failed to create Shader. Reason unkown." } };
            }
        }

        return std::unique_ptr<Shader>{ std::make_unique<VKShader>(devicePtr, module) };
    }
}