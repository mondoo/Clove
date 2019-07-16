#include "clvpch.hpp"
#include "Render2DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::ecs{
	Render2DSystem::Render2DSystem(){
		//Shader
		std::shared_ptr<gfx::Shader> quadShader = gfx::BindableFactory::createShader();
		quadShader->attachShader(gfx::ShaderType::Vertex2D);
		quadShader->attachShader(gfx::ShaderType::Pixel2D);
		//^^ note that it'll be better to have a shader on each child. So it can store the data when it's down in
		//I think we can only batch the VB / IB for now - which is still a win

		//VB
		gfx::VertexLayout layout;
		layout.add(gfx::VertexElementType::position2D).add(gfx::VertexElementType::texture2D);
		gfx::VertexBufferData bufferData(std::move(layout));
		bufferData.emplaceBack(math::Vector2f{ -0.5f, -0.5f }, math::Vector2f{ 0.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{ 0.5f, -0.5f }, math::Vector2f{ 1.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{ -0.5f,  0.5f }, math::Vector2f{ 0.0f, 1.0f });
		bufferData.emplaceBack(math::Vector2f{ 0.5f,  0.5f }, math::Vector2f{ 1.0f, 1.0f });

		std::shared_ptr<gfx::VertexBuffer> quadVBBuffer = gfx::BindableFactory::createVertexBuffer(bufferData, *quadShader);

		//IB
		indices = {
			1, 3, 0,
			3, 2, 0
		};
		std::shared_ptr<gfx::IndexBuffer> quadIBBuffer = gfx::BindableFactory::createIndexBuffer(indices);

		//Proj
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		proj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);

		quadBindables.reserve(4);
		quadBindables.emplace_back(std::move(quadShader));
		quadBindables.emplace_back(std::move(quadVBBuffer));
		quadBindables.emplace_back(std::move(quadIBBuffer));
	}

	Render2DSystem::Render2DSystem(Render2DSystem&& other) noexcept = default;

	Render2DSystem& Render2DSystem::operator=(Render2DSystem&& other) noexcept = default;

	Render2DSystem::~Render2DSystem() = default;

	void Render2DSystem::update(float deltaTime){
		for(auto& componentTuple : components){
			Transform2DComponent* transform = std::get<Transform2DComponent*>(componentTuple);
			Renderable2DComponent* renderable = std::get<Renderable2DComponent*>(componentTuple);

			math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), math::Vector3f(transform->getLocalPosition(), 0.0f));
			math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), math::Vector3f(transform->getLocalScale(), 0.0f));
			const math::Matrix4f modProj = proj * (translation * scale);

			//TODO: Atomic lock here to make the array and again when making the submit data could be a performance slow down source
			std::vector<std::shared_ptr<gfx::Bindable>> bindables = quadBindables;
			bindables.push_back(renderable->texture);

			gfx::Renderer::submitSprite(std::move(gfx::SubmitData{ static_cast<unsigned int>(indices.size()), modProj, bindables }));
		}
	}
}