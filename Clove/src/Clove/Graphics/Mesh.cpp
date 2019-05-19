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

		//Temp stuff to get it drawing
		math::Vector3f meshPos = { 0.0f, 0.0f, -4.0f };
		math::Vector3f meshRot = { 0.5f, 1.0f, 0.0f };
		math::Vector3f meshScale = { 1.0f, 1.0f, 1.0f };

		math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), meshPos);
		math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), math::asRadians(90.0f), meshRot);
		math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), meshScale);

		math::Matrix4f trans = translation * rotation * scale;

		shader->setModelMatrix(trans);
		shader->setViewMatrix(math::Matrix4f(1.0f));
		shader->setProjectionMatrix(math::createPerspectiveMatrix(45.0f, 16.0f / 9.0f, 0.5f, 50.0f));

		addBindable(std::move(shader));
	}
}