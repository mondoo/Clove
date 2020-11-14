namespace clv::gfx::vk{
    const DescriptorSetLayout::Descriptor& VKDescriptorSetLayout::getDescriptor() const {
        return descriptor;
    }

    VkDescriptorSetLayout VKDescriptorSetLayout::getLayout() const {
        return layout;
    }
}