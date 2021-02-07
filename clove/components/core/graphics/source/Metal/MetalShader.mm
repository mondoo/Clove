#include "Clove/Graphics/Metal/MetalShader.hpp"

namespace garlic::clove {
	MetalShader::MetalShader(id<MTLFunction> function)
		: function{ std::move(function) }{
	}
	
	MetalShader::MetalShader(MetalShader &&other) noexcept = default;
	
	MetalShader& MetalShader::operator=(MetalShader &&other) noexcept = default;
	
	MetalShader::~MetalShader() = default;
	
	id<MTLFunction> MetalShader::getFunction() const {
		return function;
	}
}
