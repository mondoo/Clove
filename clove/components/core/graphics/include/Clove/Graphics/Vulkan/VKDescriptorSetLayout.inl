namespace garlic::clove {
    GhaDescriptorSetLayout::Descriptor const &VKDescriptorSetLayout::getDescriptor() const {
        return descriptor;
    }

    VkDescriptorSetLayout VKDescriptorSetLayout::getLayout() const {
        return layout;
    }
}