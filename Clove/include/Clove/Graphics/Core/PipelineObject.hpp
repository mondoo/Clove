#pragma once

#include "Clove/Graphics/Core/FactoryChild.hpp"

#include "Clove/Graphics/Core/Shader.hpp"
#include "Clove/Graphics/Core/VertexLayout.hpp"

namespace clv::gfx {
	enum class CullFace {
		Front,
		Back
	};
}

namespace clv::gfx {
	class PipelineObject : public FactoryChild {
		//FUNCTIONS
	public:
		virtual ~PipelineObject() = default;

		virtual void setVertexShader(const Shader& vertexShader) = 0;
		virtual void setGeometryShader(const Shader& geometryShader) = 0;
		virtual void setPixelShader(const Shader& pixelShader) = 0;

		virtual void setBlendState(bool enabled) = 0;
		virtual void setCullMode(CullFace face, bool frontFaceCounterClockwise) = 0;

		virtual const VertexLayout& getVertexLayout() const = 0;
	};
}