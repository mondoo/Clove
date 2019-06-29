#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

#include "Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.hpp"

namespace clv::gfx{
	class Renderer;

	struct GLVertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct GLMaterialData{
		alignas(16) float sininess;
	};

	class GL4Shader : public Shader{
		//VARIABLES
	private:
		unsigned int programID = 0;

		GL4UniformBufferObject<GLVertexData> vertCB; //Not all shaders will need / use this
		GLVertexData vData;

		GL4UniformBufferObject<GLMaterialData> materialCB; //Not all shaders will need / use this
		GLMaterialData mData;

		//FUNCTIONS
	public:
		GL4Shader();
		GL4Shader(const GL4Shader& other) = delete;
		GL4Shader(GL4Shader&& other) noexcept = default;
		GL4Shader& operator=(const GL4Shader& other) = delete;
		GL4Shader& operator=(GL4Shader&& other) noexcept = default;
		~GL4Shader();

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual void attachShader(ShaderTypes type) override;

		virtual void setModelMatrix(const math::Matrix4f& model) override;

	private:
		std::string getPathForShader(ShaderTypes shader);

		std::string parseShader(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);
	};
}