#include "clvpch.hpp"
#include "Render2DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::ecs{
	Render2DSystem::Render2DSystem(){
		//Shader
		quadShader = gfx::BindableFactory::createShader();
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

		quadVBBuffer = gfx::BindableFactory::createVertexBuffer(bufferData, *quadShader);

		//IB
		indices = {
			1, 3, 0,
			3, 2, 0
		};
		quadIBBuffer = gfx::BindableFactory::createIndexBuffer(indices);

		//SBO
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		proj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);

		SBO = gfx::BindableFactory::createShaderBufferObject<ShaderData>(gfx::ShaderType::Vertex, gfx::BBP_2DData);
	}

	Render2DSystem::Render2DSystem(Render2DSystem&& other) noexcept = default;

	Render2DSystem& Render2DSystem::operator=(Render2DSystem&& other) noexcept = default;

	Render2DSystem::~Render2DSystem() = default;

	void Render2DSystem::update(float deltaTime){
		std::vector<math::Vector2f> testPos{
			{0.0f, 0.0f},
			{-500.0f, 0.0f},
			{-500.0f, 300.0f},
			{560.0f, 82.0f},
		};

		quadShader->bind();
		quadVBBuffer->bind();
		quadIBBuffer->bind();
		SBO->bind();

		for(auto& pos : testPos){
			math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), math::Vector3f(pos, 0.0f));
			math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), math::Vector3f(math::Vector2f(50, 50), 0.0f));
			data.modelProjection = proj * (translation * scale);
			SBO->update(data);
			//renderer->drawIndexed(quadIBBuffer->getIndexCount());
		}
	}
}