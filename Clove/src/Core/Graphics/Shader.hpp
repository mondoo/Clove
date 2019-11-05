#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	struct ShaderReflectionData{
		VertexLayout vertexBufferLayout;
	};
}

namespace clv::gfx{
	class Shader{
		//FUNCTIONS
	public:
		Shader() = default;
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept = default;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept = default;
		virtual ~Shader() = default;

		virtual ShaderReflectionData getReflectionData() = 0;
	};
}