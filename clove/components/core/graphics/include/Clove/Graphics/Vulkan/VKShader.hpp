#pragma once

#include "Clove/Graphics/Shader.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <span>
#include <string_view>
#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKShader : public Shader {
        //VARIABLES
    private:
        DevicePointer device;

        VkShaderModule module{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKShader() = delete;
        VKShader(DevicePointer device, std::string_view filePath);
        VKShader(DevicePointer device, std::span<std::byte const> byteCode);

        VKShader(VKShader const &other) = delete;
        VKShader(VKShader &&other) noexcept;

        VKShader &operator=(VKShader const &other) = delete;
        VKShader &operator=(VKShader &&other) noexcept;

        ~VKShader();

        VkShaderModule getModule() const;

        static VkShaderStageFlags convertStage(Stage stage);
    };
}