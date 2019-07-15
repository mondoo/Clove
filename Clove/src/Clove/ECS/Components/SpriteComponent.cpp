#include "clvpch.hpp"
#include "SpriteComponent.hpp"

#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
//Temp
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::ecs{
	SpriteComponent::SpriteComponent(){
		//Temp - moved here until bindables are shared_ptrs
		//Shader
		std::shared_ptr<gfx::Shader> quadShader = gfx::BindableFactory::createShader();
		quadShader->attachShader(gfx::ShaderType::Vertex2D);
		quadShader->attachShader(gfx::ShaderType::Pixel2D);

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
		//const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		//const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		//proj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);

		quadBindables.reserve(4);
		quadBindables.emplace_back(std::move(quadShader));
		quadBindables.emplace_back(std::move(quadVBBuffer));
		quadBindables.emplace_back(std::move(quadIBBuffer));
		//quadBindables.emplace_back(std::move(tex));
	}

	SpriteComponent::SpriteComponent(SpriteComponent&& other) noexcept = default;

	SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) noexcept = default;

	SpriteComponent::~SpriteComponent() = default;

	void SpriteComponent::setTexture(const std::string& path){
		//Renderable2DComponent::setTexture(gfx::BindableFactory::createTexture(path, gfx::TBP_Sprite));
		quadBindables.emplace_back(gfx::BindableFactory::createTexture(path, gfx::TBP_Sprite));
	}
}