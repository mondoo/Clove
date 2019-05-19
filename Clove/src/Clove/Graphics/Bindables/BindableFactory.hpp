#pragma once

namespace clv::gfx{
	class Bindable;
	class IndexBuffer;
	class Shader;
	class VertexBufferLayout;
	class Renderer;

	class BindableFactory{
		//FUNCTIONS
	public:
		static std::unique_ptr<Bindable>			createVertexBuffer(const std::vector<float>& vertexData, Renderer& renderer);
		static std::unique_ptr<IndexBuffer>			createIndexBuffer(const std::vector<unsigned int>& indexData, Renderer& renderer);
		static std::unique_ptr<Shader>				createShader(Renderer& renderer);
		static std::unique_ptr<VertexBufferLayout>	createVertexBufferLayout(Renderer& renderer);
	};
}