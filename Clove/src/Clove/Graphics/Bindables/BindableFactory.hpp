#pragma once

namespace clv::gfx{
	class Bindable;
	class IndexBuffer;
	class Shader;
	class VertexBufferLayout;
	class Renderer;
	class Texture;
}

namespace clv::gfx::BindableFactory{
	std::unique_ptr<Bindable>			createVertexBuffer(const std::vector<float>& vertexData);
	std::unique_ptr<IndexBuffer>		createIndexBuffer(const std::vector<unsigned int>& indexData);
	std::unique_ptr<Shader>				createShader();
	std::unique_ptr<VertexBufferLayout>	createVertexBufferLayout();
	std::unique_ptr<Texture>			createTexture(const std::string& filePath);
};