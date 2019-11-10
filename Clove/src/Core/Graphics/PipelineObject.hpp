#pragma once

#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	class PipelineObject{
		//FUNCTIONS
	public:
		virtual const std::shared_ptr<Shader>& getShader() const = 0;
		virtual const VertexLayout& getVertexLayout() const = 0;
	};
}