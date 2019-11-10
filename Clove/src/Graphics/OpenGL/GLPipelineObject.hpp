#pragma once

#include "Core/Graphics/PipelineObject.hpp"

namespace clv::gfx::ogl{
	class GLPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;

		//FUNCTIONS
	public:
		GLPipelineObject() = delete;
		GLPipelineObject(const std::shared_ptr<Shader>& shader);
		GLPipelineObject(const GLPipelineObject& other) = delete;
		GLPipelineObject(GLPipelineObject&& other);
		GLPipelineObject& operator=(const GLPipelineObject& other) = delete;
		GLPipelineObject& operator=(GLPipelineObject&& other);
		~GLPipelineObject();

		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;
	};
}