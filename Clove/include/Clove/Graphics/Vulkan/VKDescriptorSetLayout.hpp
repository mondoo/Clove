#pragma once

//TODO: Move
#include <vulkan/vulkan.h>

namespace clv::gfx {
    enum class DescriptorType {
        UniformBuffer,
    };

    enum class DescriptorStage {
        Vertex,
    };

    struct DescriptorBindingDescriptor {
        uint32_t binding = 0;
        DescriptorType type;
        size_t arraySize = 0;
        DescriptorStage stage;
    };

    struct DescriptorSetLayoutDescriptor {
        std::vector<DescriptorBindingDescriptor> bindings;
    };
}

namespace clv::gfx::vk {
    //Describes all of the descriptors that the shaders will use
    class VKDescriptorSetLayout {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkDescriptorSetLayout layout = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKDescriptorSetLayout(VkDevice device, DescriptorSetLayoutDescriptor descriptor);
        ~VKDescriptorSetLayout();

        VkDescriptorSetLayout getLayout() const;
    };
}