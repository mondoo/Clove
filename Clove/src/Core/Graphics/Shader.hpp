#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	struct ShaderReflectionData{
		VertexLayout vertexBufferLayout;
	};

	class Shader{
		//FUNCTIONS
	public:
		Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;
		virtual ~Shader();

		virtual ShaderReflectionData getReflectionData() = 0;
	};
}