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
		std::unique_ptr<gfx::Shader> quadShader = gfx::BindableFactory::createShader();
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

		std::unique_ptr<gfx::VertexBuffer> quadVBBuffer = gfx::BindableFactory::createVertexBuffer(bufferData, *quadShader);

		//IB
		indices = {
			1, 3, 0,
			3, 2, 0
		};
		std::unique_ptr<gfx::IndexBuffer> quadIBBuffer = gfx::BindableFactory::createIndexBuffer(indices);

		//SBO
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		proj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);

		//Test texture
		std::unique_ptr<gfx::Texture> tex = gfx::BindableFactory::createTexture("res/Textures/Zombie-32x32.png", gfx::TBP_Sprite);

		quadBindables.reserve(4);
		quadBindables.emplace_back(std::move(quadShader));
		quadBindables.emplace_back(std::move(quadVBBuffer));
		quadBindables.emplace_back(std::move(quadIBBuffer));
		quadBindables.emplace_back(std::move(tex));
	}

	Render2DSystem::Render2DSystem(Render2DSystem&& other) noexcept = default;

	Render2DSystem& Render2DSystem::operator=(Render2DSystem&& other) noexcept = default;

	Render2DSystem::~Render2DSystem() = default;

	void Render2DSystem::update(float deltaTime){
		std::vector<math::Vector2f> testPos{
			{0.0f, 0.0f},
			{-15.0f, 0.0f},
			{-500.0f, 300.0f},
			{560.0f, 82.0f},
		};

		for(auto& pos : testPos){
			math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), math::Vector3f(pos, 0.0f));
			math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), math::Vector3f(math::Vector2f(50, 50), 0.0f));
			const math::Matrix4f modProj = proj * (translation * scale);

			gfx::Renderer::submitSprite(std::move(gfx::SubmitData{ static_cast<unsigned int>(indices.size()), modProj, quadBindables}));
		}
	}
}