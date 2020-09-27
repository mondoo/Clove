#pragma once

#include "Clove/Graphics/Shader.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>
#include <span>

namespace clv::gfx::vk {
    class VKShader : public Shader {
        //VARIABLES
    private:
        DevicePointer device;

        VkShaderModule module{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKShader() = delete;
        VKShader(DevicePointer device, std::string_view filePath);
        VKShader(DevicePointer device, std::span<const std::byte> byteCode);

        VKShader(const VKShader& other) = delete;
        VKShader(VKShader&& other) noexcept;

        VKShader& operator=(const VKShader& other) = delete;
        VKShader& operator=(VKShader&& other) noexcept;

        ~VKShader();

        VkShaderModule getModule() const;
    };
}