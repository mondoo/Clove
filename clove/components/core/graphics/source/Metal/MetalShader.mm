#include "Clove/Graphics/Metal/MetalShader.hpp"

namespace garlic::clove {
    MetalShader::MetalShader(id<MTLFunction> function)
        : function{ function }{
    }
    
    MetalShader::MetalShader(MetalShader &&other) noexcept = default;
    
    MetalShader& MetalShader::operator=(MetalShader &&other) noexcept = default;
    
    MetalShader::~MetalShader() = default;
}
