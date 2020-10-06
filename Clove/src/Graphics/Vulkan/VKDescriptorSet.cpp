#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKSampler.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"
#include "Clove/Utils/Cast.hpp"

namespace clv::gfx::vk {
    VKDescriptorSet::VKDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet)
        : device(device)
        , descriptorSet(descriptorSet) {
    }

    VKDescriptorSet::VKDescriptorSet(VKDescriptorSet&& other) noexcept = default;

    VKDescriptorSet& VKDescriptorSet::operator=(VKDescriptorSet&& other) noexcept = default;

    VKDescriptorSet::~VKDescriptorSet() = default;

    void VKDescriptorSet::write(const GraphicsBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot) {
        const auto* vkBuffer = polyCast<const VKBuffer>(&buffer);

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = vkBuffer->getBuffer();
        bufferInfo.offset = offset;
        bufferInfo.range  = range;

        VkWriteDescriptorSet writeInfo{};
        writeInfo.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.pNext            = nullptr;
        writeInfo.dstSet           = descriptorSet;
        writeInfo.dstBinding       = bindingSlot;
        writeInfo.dstArrayElement  = 0;
        writeInfo.descriptorCount  = 1;
        writeInfo.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeInfo.pImageInfo       = nullptr;
        writeInfo.pBufferInfo      = &bufferInfo;
        writeInfo.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
    }

    void VKDescriptorSet::write(const GraphicsImageView& imageView, const Sampler& sampler, const ImageLayout layout, const uint32_t bindingSlot) {
        const auto* vkSampler   = polyCast<const VKSampler>(&sampler);
        const auto* vkImageView = polyCast<const VKImageView>(&imageView);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.sampler     = vkSampler->getSampler();
        imageInfo.imageView   = vkImageView->getImageView();
        imageInfo.imageLayout = convertImageLayout(layout);

        VkWriteDescriptorSet writeInfo{};
        writeInfo.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.pNext            = nullptr;
        writeInfo.dstSet           = descriptorSet;
        writeInfo.dstBinding       = bindingSlot;
        writeInfo.dstArrayElement  = 0;
        writeInfo.descriptorCount  = 1;
        writeInfo.descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeInfo.pImageInfo       = &imageInfo;
        writeInfo.pBufferInfo      = nullptr;
        writeInfo.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
    }

    void VKDescriptorSet::write(std::span<std::shared_ptr<GraphicsImageView>> imageViews, const Sampler& sampler, const ImageLayout layout, const uint32_t bindingSlot) {
        const auto* vkSampler = polyCast<const VKSampler>(&sampler);
        std::vector<VkDescriptorImageInfo> imageInfos(std::size(imageViews));

        for(size_t i = 0; i < std::size(imageInfos); ++i) {
            VkDescriptorImageInfo imageInfo{
                .sampler     = vkSampler->getSampler(),
                .imageView   = polyCast<const VKImageView>(imageViews[i].get())->getImageView(),
                .imageLayout = convertImageLayout(layout),
            };
            imageInfos[i] = std::move(imageInfo);
        }

        VkWriteDescriptorSet writeInfo{
            .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext            = nullptr,
            .dstSet           = descriptorSet,
            .dstBinding       = bindingSlot,
            .dstArrayElement  = 0,
            .descriptorCount  = static_cast<uint32_t>(std::size(imageInfos)),
            .descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo       = std::data(imageInfos),
            .pBufferInfo      = nullptr,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
    }

    VkDescriptorSet VKDescriptorSet::getDescriptorSet() const {
        return descriptorSet;
    }
}