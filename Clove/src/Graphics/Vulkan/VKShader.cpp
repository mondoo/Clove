#include "Clove/Graphics/Vulkan/VKShader.hpp"

namespace clv::gfx::vk {
    static std::vector<std::byte> readFile(std::string_view filePath) {
        //Start at the end so we can get the file size
        std::basic_ifstream<std::byte> file(filePath.data(), std::ios::ate | std::ios::binary);

        if(!file.is_open()) {
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "Failed to open file");
            return {};
        }

        const size_t fileSize = file.tellg();
        std::vector<std::byte> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VKShader::VKShader(VkDevice device, std::string_view filePath)
        : VKShader(device, readFile(filePath)) {
    }

    VKShader::VKShader(VkDevice device, std::vector<std::byte> byteCode)
        : device(device) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext    = nullptr;
        createInfo.flags    = 0;
        createInfo.codeSize = std::size(byteCode);
        createInfo.pCode    = reinterpret_cast<uint32_t*>(std::data(byteCode));

        if(vkCreateShaderModule(device, &createInfo, nullptr, &module) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create shader module");
        }
    }

    VKShader::VKShader(VKShader&& other) noexcept = default;

    VKShader& VKShader::operator=(VKShader&& other) noexcept = default;

    VKShader::~VKShader() {
        vkDestroyShaderModule(device, module, nullptr);
    }

    VkShaderModule VKShader::getModule() const {
        return module;
    }
}
