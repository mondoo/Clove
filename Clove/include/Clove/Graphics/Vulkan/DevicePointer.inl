namespace clv::gfx::vk {
    VkDevice DevicePointer::get() const {
        return logicalDevice;
    }

    VkPhysicalDevice DevicePointer::getPhysical() const {
        return VkPhysicalDevice();
    }

    VkSurfaceKHR DevicePointer::getSurface() const {
        return VkSurfaceKHR();
    }
}