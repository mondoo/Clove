#pragma once

#include "Clove/Graphics/Shader.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

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
        VKShader(DevicePointer device, std::vector<std::byte> byteCode);
        VKShader(DevicePointer device, const std::byte* byteCode, const size_t numBytes);

        VKShader(const VKShader& other) = delete;
        VKShader(VKShader&& other) noexcept;

        VKShader& operator=(const VKShader& other) = delete;
        VKShader& operator=(VKShader&& other) noexcept;

        ~VKShader();

        VkShaderModule getModule() const;
    };
}