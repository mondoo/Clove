#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"

namespace clv::gfx::vk {
    VKDescriptorSet::VKDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet)
        : device(device)
        , descriptorSet(descriptorSet) {
    }

    void VKDescriptorSet::writeFrom(VKBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot) {
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
        writeInfo.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//TODO: Allow different types
        writeInfo.pImageInfo       = nullptr;
        writeInfo.pBufferInfo      = &bufferInfo;
        writeInfo.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
    }

    VkDescriptorSet VKDescriptorSet::getDescriptorSet() const {
        return descriptorSet;
    }
}