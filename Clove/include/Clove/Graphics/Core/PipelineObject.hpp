#pragma once

#include "Clove/Graphics/Core/Shader.hpp"
#include "Clove/Graphics/Core/VertexLayout.hpp"

namespace clv::gfx{
	class PipelineObject{
		//FUNCTIONS
	public:
		virtual ~PipelineObject() = default;

		virtual const std::shared_ptr<Shader>& getShader() const = 0;
		virtual const VertexLayout& getVertexLayout() const = 0;
	};
}