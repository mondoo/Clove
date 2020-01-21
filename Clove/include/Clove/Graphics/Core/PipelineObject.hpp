#pragma once

#include "Clove/Graphics/Core/Shader.hpp"
#include "Clove/Graphics/Core/VertexLayout.hpp"

namespace clv::gfx{
	enum class CullFace{
		Front,
		Back
	};
}

namespace clv::gfx{
	class PipelineObject{
		//FUNCTIONS
	public:
		virtual ~PipelineObject() = default;

		virtual void setBlendState(bool enabled) = 0;
		virtual void setCullMode(CullFace face, bool frontFaceCounterClockwise) = 0;

		virtual const std::shared_ptr<Shader>& getShader() const = 0;
		virtual const VertexLayout& getVertexLayout() const = 0;
	};
}