#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/RenderAPI.hpp"

namespace clv::gfx{
	class VertexBuffer;
	class VertexBufferData;
	class IndexBuffer;
	class Shader;
	class VertexLayout;
	class Renderer;
	class Texture;
	class FrameBuffer;
	template<typename T> class ShaderBufferObject;
}

namespace clv::gfx::BindableFactory{
	std::shared_ptr<VertexBuffer>			createVertexBuffer(const VertexBufferData& bufferData, Shader& shader);
	std::shared_ptr<IndexBuffer>			createIndexBuffer(const std::vector<unsigned int>& indexData);
	std::shared_ptr<Shader>					createShader();
	std::shared_ptr<Texture>				createTexture(const std::string& filePath, unsigned int bindingPoint);
	std::shared_ptr<FrameBuffer>			createFrameBuffer();

	template<typename T>
	std::shared_ptr<ShaderBufferObject<T>>	createShaderBufferObject(ShaderType shaderType, unsigned int bindingPoint);
};

#include "BindableFactory.inl"