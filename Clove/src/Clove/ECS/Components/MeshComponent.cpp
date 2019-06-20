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

#include "Clove/Graphics/VertexLayout.hpp"

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

		gfx::VertexLayout layout;
		layout.add(gfx::VertexElementType::position3D).add(gfx::VertexElementType::texture2D).add(gfx::VertexElementType::normal);
		gfx::VertexArray vertexArray{ std::move(layout) };
		for(int i = 0; i < info.verticies.size(); ++i){
			//Should the original vertex buffer be removed and inherit from this new one?
			//---no it should to the vertex array (really need a better name because it conflicts with opengl)


			//so it should go something like this

			//idea 1
			//The type of shader that is used is used to create the layout (the shader itself isn't responsible for creating this
			//We then use the layout to fill the array
			//The array is then passed off to the buffers
			//Inside the buffers is where the input layout / gl vertex array is made?

			//idea 2
			//We make the layout for GL just the data needed for the layout and we make the layout for DX the input layout
			//We make the vertex array the array for GL and empty for DX

			//To make this thing work we need to make it like one system
			//currently there are differences between clove's dx/gl api
			//We need to make it either work like DX or work like GL (no point in working against both apis)
			//---We should make it like dx since that's kind of the one it is most like at the moment
			//------the only problem is the vertex array shinanagins 
			//---Although making it like opengl would be easier because the layout is described in the vertex array so we can just define the input layout in there instead

			//are we going to have to abstract out the renderable itself?
			//---no shouldn't need to


			//NOTE
			//Take a page out of that other guy's book: What would I like the code to look like?

			//idea 3 - we make it like opengl
			//Vertex array takes the layout description and shader
			//We then fill the vertex buffer(s) and bind them to the array (might need to fill them first)
			//---how does the new system fit into this?

			//I think the main thing to take away is that perhaps having the layout description as a bindable was the wrong way to go
			//if it has to be a bindable (like in dx case) then it can be handled inside another bindable (like the buffer or the array)
			//It's quite diffiucult to make this new system fit in at the moment because it is so different to everything else
			//I think the best course right now is to refactor the system to remove the jank api call and then try and ham fist the new system in?
			//or maybe pick parts from it?
			//or merge it into other things?
			//either way - the layout should be how it is in the new system. A ver ysimple structure that just defines the layout of the vertex buffer (my initial implementation was too complicated / had too much responsibility)

			const auto pos = math::Vector3f{
					info.verticies[i].x,
					info.verticies[i].y,
					info.verticies[i].z
			};

			//Note we can always do empty structs here if the model doesn't have the data
			//The layout should depend on the shader and then we just fill data
			vertexArray.emplaceBack(
				pos,
				math::Vector2f{
					info.texCoords[i].x,
					info.texCoords[i].y
				},
				math::Vector2f{
					info.normals[i].x,
					info.normals[i].y
				}

				//Testing what happens if too many are added
				/*,math::Vector2f{
					info.normals[i].x,
					info.normals[i].y
				}*/
			);
		}

		for(const auto i : info.indices){
			indices.push_back(i);
		}

		//VB
		//std::unique_ptr<gfx::VertexBuffer> vertexBuffer = gfx::BindableFactory::createVertexBuffer(vertices);

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

		//addBindable(std::move(vertexBuffer));
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