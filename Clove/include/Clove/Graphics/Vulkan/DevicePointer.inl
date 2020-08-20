namespace clv::gfx::vk {
    VkDevice DevicePointer::get() const {
        return logicalDevice;
    }
}