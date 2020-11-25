namespace garlic::clove {
    DescriptorSetLayout::Descriptor const &VKDescriptorSetLayout::getDescriptor() const {
        return descriptor;
    }

    VkDescriptorSetLayout VKDescriptorSetLayout::getLayout() const {
        return layout;
    }
}