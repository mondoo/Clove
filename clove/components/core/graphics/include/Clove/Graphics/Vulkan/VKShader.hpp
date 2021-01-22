#pragma once

#include "Clove/Graphics/GhaShader.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <span>
#include <string_view>
#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKShader : public GhaShader {
        //VARIABLES
    private:
        DevicePointer device;

        VkShaderModule module{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKShader() = delete;
        VKShader(DevicePointer device, VkShaderModule module);

        VKShader(VKShader const &other) = delete;
        VKShader(VKShader &&other) noexcept;

        VKShader &operator=(VKShader const &other) = delete;
        VKShader &operator=(VKShader &&other) noexcept;

        ~VKShader();

        VkShaderModule getModule() const;

        static VkShaderStageFlags convertStage(Stage stage);
    };
}