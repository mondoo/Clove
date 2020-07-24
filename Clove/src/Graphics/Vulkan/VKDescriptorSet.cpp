#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKSampler.hpp"
#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    VKDescriptorSet::VKDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet)
        : device(device)
        , descriptorSet(descriptorSet) {
    }

    void VKDescriptorSet::writeFrom(const VKBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer.getBuffer();
        bufferInfo.offset = offset;
        bufferInfo.range  = range;

        VkWriteDescriptorSet writeInfo{};
        writeInfo.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.pNext            = nullptr;
        writeInfo.dstSet           = descriptorSet;
        writeInfo.dstBinding       = bindingSlot;
        writeInfo.dstArrayElement  = 0;
        writeInfo.descriptorCount  = 1;
        writeInfo.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//TODO: Allow different buffer types
        writeInfo.pImageInfo       = nullptr;
        writeInfo.pBufferInfo      = &bufferInfo;
        writeInfo.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
    }

    void VKDescriptorSet::writeFrom(const VKSampler& sampler, const VKImageView& imageView, const ImageLayout layout, const uint32_t bindingSlot) {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.sampler     = sampler.getSampler();
        imageInfo.imageView   = imageView.getImageView();
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

    VkDescriptorSet VKDescriptorSet::getDescriptorSet() const {
        return descriptorSet;
    }
}