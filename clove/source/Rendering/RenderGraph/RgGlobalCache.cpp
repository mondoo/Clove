#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"

namespace garlic::clove {
    std::shared_ptr<GhaShader> RgGlobalCache::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaShader> RgGlobalCache::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaRenderPass> RgGlobalCache::createRenderPass(GhaRenderPass::Descriptor descriptor) {
        //TODO
        return nullptr;
    }
}
