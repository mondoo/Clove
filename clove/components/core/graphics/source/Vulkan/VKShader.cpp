#include "Clove/Graphics/Vulkan/VKShader.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <fstream>

namespace garlic::clove {
    static std::vector<std::byte> readFile(std::string_view filePath) {
        //Start at the end so we can get the file size
        std::basic_ifstream<std::byte> file(filePath.data(), std::ios::ate | std::ios::binary);

        if(!file.is_open()) {
            GARLIC_LOG(garlicLogContext, LogLevel::Error, "{0}: Failed to open file", GARLIC_FUNCTION_NAME);
            return {};
        }

        size_t const fileSize = file.tellg();
        std::vector<std::byte> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VKShader::VKShader(DevicePointer device, std::string_view filePath)
        : VKShader(std::move(device), readFile(filePath)) {
    }

    VKShader::VKShader(DevicePointer device, std::span<std::byte const> byteCode)
        : device(std::move(device)) {
        VkShaderModuleCreateInfo createInfo{
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext    = nullptr,
            .flags    = 0,
            .codeSize = byteCode.size_bytes(),
            .pCode    = reinterpret_cast<uint32_t const *>(std::data(byteCode)),
        };

        if(vkCreateShaderModule(this->device.get(), &createInfo, nullptr, &module) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, LogLevel::Error, "Failed to create shader module");
        }
    }

    VKShader::VKShader(VKShader &&other) noexcept = default;

    VKShader &VKShader::operator=(VKShader &&other) noexcept = default;

    VKShader::~VKShader() {
        vkDestroyShaderModule(device.get(), module, nullptr);
    }

    VkShaderModule VKShader::getModule() const {
        return module;
    }

    VkShaderStageFlags VKShader::convertStage(Stage stage) {
        VkShaderStageFlags vkStage{ 0 };
        if((stage & Shader::Stage::Vertex) != 0) {
            vkStage |= VK_SHADER_STAGE_VERTEX_BIT;
        }
        if((stage & Shader::Stage::Pixel) != 0) {
            vkStage |= VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        return vkStage;
    }
}
