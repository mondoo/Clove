#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class VertexBuffer;
	struct Vertex;
	class IndexBuffer;
	class Shader;
	class VertexBufferLayout;
	class Renderer;
	class Texture;
	template<typename T> class ShaderBufferObject;
}

namespace clv::gfx::BindableFactory{
	std::unique_ptr<VertexBuffer>			createVertexBuffer(const std::vector<Vertex>& vertexData);
	std::unique_ptr<IndexBuffer>			createIndexBuffer(const std::vector<unsigned int>& indexData);
	std::unique_ptr<Shader>					createShader();
	std::unique_ptr<VertexBufferLayout>		createVertexBufferLayout();
	std::unique_ptr<Texture>				createTexture(const std::string& filePath, unsigned int bindingPoint);

	template<typename T>
	std::unique_ptr<ShaderBufferObject<T>>	createShaderBufferObject(ShaderTypes shaderType, unsigned int bindingPoint);
};

#include "BindableFactory.inl"