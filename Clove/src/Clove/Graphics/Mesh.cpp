#include "clvpch.hpp"
#include "Mesh.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindables/Bindable.hpp"
#include "Clove/Graphics/Bindables/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/VertexBufferLayout.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Utils/MeshLoader.hpp"

namespace clv::gfx{
	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

	Mesh::Mesh(){
		loader::MeshInfo info = loader::MeshLoader::loadOBJ("res/Objects/cube.obj");

		for(const auto i : info.indices){
			indices.push_back(i);
		}

		//VB
		std::unique_ptr<Bindable> vertexBuffer = BindableFactory::createVertexBuffer(info.vertexData);

		//IB
		addIndexBuffer(BindableFactory::createIndexBuffer(indices));

		//Shader
		std::unique_ptr<Shader> shader = BindableFactory::createShader();
		shader->attachShader(ShaderTypes::Vertex);
		shader->attachShader(ShaderTypes::Pixel);
		shader->bind(Application::get().getWindow().getRenderer());
		this->shader = shader.get();

		//VBL (maybe call this a VBO?)
		std::unique_ptr<VertexBufferLayout> layout = BindableFactory::createVertexBufferLayout();
		layout->pushElement("Position", BufferElementFormat::FLOAT_3);
		layout->pushElement("TexCoord", BufferElementFormat::FLOAT_2);
		layout->pushElement("Normal", BufferElementFormat::FLOAT_3);
		switch(Application::get().getWindow().getRenderer().getAPI()){//TODO: how to remove this check?
			case API::OpenGL4:
				layout->createLayout(*vertexBuffer);
				break;
			case API::DirectX11:
				layout->createLayout(*shader);
				break;
		}

		addBindable(std::move(vertexBuffer));
		addBindable(std::move(layout));
		addBindable(std::move(shader));
	}

	void Mesh::setModelMatrix(const math::Matrix4f& model){
		shader->setModelMatrix(model);
	}

	void Mesh::setDiffuseTexture(const std::string& path){
		addBindable(BindableFactory::createTexture(path, gfx::TBP_Diffuse));
	}

	void Mesh::setSpecularTexture(const std::string& path){
		addBindable(BindableFactory::createTexture(path, gfx::TBP_Specular));
	}

	/*void Mesh::setTexture(std::unique_ptr<Texture> texture){
		addBindable(std::move(texture));
	}*/
}