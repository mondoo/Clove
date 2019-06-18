#include "clvpch.hpp"
#include "MeshComponent.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Context.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
//#include "Clove/Graphics/Bindables/VertexBufferLayout.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Utils/MeshLoader.hpp"

namespace clv::ecs{
	MeshComponent::MeshComponent() = default;

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept = default;

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept = default;

	MeshComponent::~MeshComponent() = default;

	void MeshComponent::setModelMatrix(const math::Matrix4f& model){
		shader->setModelMatrix(model);
	}

	void MeshComponent::setMesh(const std::string& filePath){
		loader::MeshInfo info = loader::MeshLoader::loadOBJ(filePath);

		//TODO: This will break if the mesh does not have a texture mapping or normal mapping
		for(int i = 0; i < info.verticies.size(); ++i){
			vertices.push_back(
				{
					info.verticies[i].x, info.verticies[i].y, info.verticies[i].z,
					info.texCoords[i].x, info.texCoords[i].y,
					info.normals[i].x, info.normals[i].y, info.normals[i].z
				}
			);
		}

		for(const auto i : info.indices){
			indices.push_back(i);
		}

		//VB
		std::unique_ptr<gfx::VertexBuffer> vertexBuffer = gfx::BindableFactory::createVertexBuffer(vertices);

		//IB
		addIndexBuffer(gfx::BindableFactory::createIndexBuffer(indices));

		//Shader
		std::unique_ptr<gfx::Shader> shader = gfx::BindableFactory::createShader();
		shader->attachShader(gfx::ShaderTypes::Vertex);
		shader->attachShader(gfx::ShaderTypes::Pixel);
		shader->bind(Application::get().getRenderer());
		this->shader = shader.get();

		//VBL (maybe call this a VBO?)
		//std::unique_ptr<gfx::VertexLayout> layout = gfx::BindableFactory::createVertexBufferLayout();
		//layout->pushElement("Position", gfx::BufferElementFormat::FLOAT_3);
		//layout->pushElement("TexCoord", gfx::BufferElementFormat::FLOAT_2);
		//layout->pushElement("Normal", gfx::BufferElementFormat::FLOAT_3);
		//switch(Application::get().getWindow().getContext().getAPI()){//TODO: how to remove this check?
		//	case gfx::API::OpenGL4:
		//		layout->createLayout(*vertexBuffer);
		//		break;
		//	#if CLV_PLATFORM_WINDOWS
		//	case gfx::API::DirectX11:
		//		layout->createLayout(*shader);
		//		break;
		//	#endif
		//}

		addBindable(std::move(vertexBuffer));
		//addBindable(std::move(layout));
		addShader(std::move(shader));
	}

	void MeshComponent::setDiffuseTexture(const std::string& path){
		addBindable(gfx::BindableFactory::createTexture(path, gfx::TBP_Diffuse));
	}

	void MeshComponent::setSpecularTexture(const std::string& path){
		addBindable(gfx::BindableFactory::createTexture(path, gfx::TBP_Specular));
	}
}