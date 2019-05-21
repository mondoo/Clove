#include "clvpch.hpp"
#include "Mesh.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindables/Bindable.hpp"
#include "Clove/Graphics/Bindables/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/VertexBufferLayout.hpp"
#include "Clove/Utils/MeshLoader.hpp"

namespace clv::gfx{
	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

	Mesh::Mesh(Renderer& renderer){
		loader::MeshInfo info = loader::MeshLoader::loadOBJ("res/Objects/cube.obj");

		for(const auto i : info.indices){
			indices.push_back(i);
		}

		for(const auto& v : info.verticies){
			vertices.push_back(v.x);
			vertices.push_back(v.y);
			vertices.push_back(v.z);
		}

		//VB
		std::unique_ptr<Bindable> vertexBuffer = BindableFactory::createVertexBuffer(vertices, renderer);

		//IB
		addIndexBuffer(BindableFactory::createIndexBuffer(indices, renderer));

		//Shader
		std::unique_ptr<Shader> shader = BindableFactory::createShader(renderer);
		shader->attachShader(ShaderTypes::Vertex, renderer);
		shader->attachShader(ShaderTypes::Pixel, renderer);
		shader->bind(renderer);
		this->shader = shader.get();

		//VBL (maybe call this a VBO?)
		std::unique_ptr<VertexBufferLayout> layout = BindableFactory::createVertexBufferLayout(renderer);
		layout->pushElement("Position", BufferElementFormat::FLOAT_3);
		switch(renderer.getAPI()){//TODO: how to remove this check?
			case API::OpenGL:
				layout->createLayout(*vertexBuffer, renderer);
				break;
			case API::DirectX11:
				layout->createLayout(*shader, renderer);
				break;
		}
		addBindable(std::move(vertexBuffer));
		addBindable(std::move(layout));
		addBindable(std::move(shader));
	}

	void Mesh::setWorldMatrix(const math::Matrix4f& world){
		shader->setWorldMatrix(world);
	}

	void Mesh::setViewMatrix(const math::Matrix4f& view){
		shader->setViewMatrix(view);
	}

	void Mesh::setProjection(const math::Matrix4f& projection){
		shader->setProjectionMatrix(projection);
	}
}