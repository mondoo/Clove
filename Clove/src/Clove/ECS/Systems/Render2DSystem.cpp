#include "clvpch.hpp"
#include "Render2DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::ecs{
	Render2DSystem::Render2DSystem() = default;

	Render2DSystem::Render2DSystem(Render2DSystem&& other) noexcept = default;

	Render2DSystem& Render2DSystem::operator=(Render2DSystem&& other) noexcept = default;

	Render2DSystem::~Render2DSystem() = default;

	void Render2DSystem::update(float deltaTime){
		quadShader->bind(*renderer);
		quadVBBuffer->bind(*renderer);
		quadIBBuffer->bind(*renderer);

		renderer->drawIndexed(quadIBBuffer->getIndexCount());
	}

	void Render2DSystem::initialiseRenderer(const std::shared_ptr<gfx::Renderer>& renderer){
		this->renderer = renderer;

		//Temp? Needs window and that to be initialised

		/*
		TODO:
		Setting up 2D shaders but the shader system is too coupled to the current shaders
		so this will need to be refactored before continuing
		*/

		//Shader
		quadShader = gfx::BindableFactory::createShader();
		quadShader->attachShader(gfx::ShaderType::Vertex2D);
		quadShader->attachShader(gfx::ShaderType::Pixel2D);

		//VB
		gfx::VertexLayout layout;
		layout.add(gfx::VertexElementType::position2D).add(gfx::VertexElementType::texture2D);
		gfx::VertexBufferData bufferData(std::move(layout));
		bufferData.emplaceBack(math::Vector2f{-0.5f, -0.5f }, math::Vector2f{ 0.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{ 0.5f, -0.5f }, math::Vector2f{ 1.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{-0.5f,  0.5f }, math::Vector2f{ 0.0f, 1.0f });
		bufferData.emplaceBack(math::Vector2f{ 0.5f,  0.5f }, math::Vector2f{ 1.0f, 1.0f });

		quadVBBuffer = gfx::BindableFactory::createVertexBuffer(bufferData, *quadShader);

		//IB
		indices = {
			0, 2, 3,
			0, 3, 1
		};
		quadIBBuffer = gfx::BindableFactory::createIndexBuffer(indices);

	}
}