#include "Clove/Graphics/Vulkan/VKShader.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <vector>

namespace garlic::clove {
    VKShader::VKShader(DevicePointer device, VkShaderModule module)
        : device{ std::move(device) }
        , module{ module } {
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
        if((stage & GhaShader::Stage::Vertex) != 0) {
            vkStage |= VK_SHADER_STAGE_VERTEX_BIT;
        }
        if((stage & GhaShader::Stage::Pixel) != 0) {
            vkStage |= VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        return vkStage;
    }
}
