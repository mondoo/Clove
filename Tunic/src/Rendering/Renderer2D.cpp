#include "Tunic/Rendering/Renderer2D.hpp"

#include "Tunic/Rendering/Renderables/Sprite.hpp"

#include <Clove/Graphics/Core/CommandBuffer.hpp>
#include <Clove/Graphics/Core/GraphicsFactory.hpp>
#include <Clove/Graphics/Core/PipelineObject.hpp>
#include <Clove/Graphics/Core/Resources/Buffer.hpp>
#include <Clove/Graphics/Core/Surface.hpp>
#include <Clove/Graphics/Core/VertexLayout.hpp>
#include <Clove/Platform/Core/Window.hpp>

using namespace clv;
using namespace clv::gfx;

namespace tnc::rnd {
	Renderer2D::Renderer2D(clv::plt::Window& window) 
		: renderTarget(window.getSurface()->getRenderTarget())
		, screenSize(window.getSize()) {
		GraphicsFactory& factory = *window.getGraphicsFactory();

		commandBuffer = factory.createCommandBuffer();

		//Default
		auto defaultVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/Default2D-vs.hlsl");
		auto defaultPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/Default2D-ps.hlsl");
		defaultPipelineObject = factory.createPipelineObject();
		defaultPipelineObject->setVertexShader(*defaultVS);
		defaultPipelineObject->setPixelShader(*defaultPS);

		//Text
		auto textVS = factory.createShader({ ShaderStage::Vertex }, "res/Shaders/Font-vs.hlsl");
		auto textPS = factory.createShader({ ShaderStage::Pixel }, "res/Shaders/Font-ps.hlsl");
		textPipelineObject = factory.createPipelineObject();
		textPipelineObject->setVertexShader(*textVS);
		textPipelineObject->setPixelShader(*textPS);

		//Buffers
		const std::vector<uint32_t> indices{
			0, 1, 2,
			0, 2, 3
		};
		BufferDescriptor ibDesc{};
		ibDesc.elementSize	= sizeof(uint32_t);
		ibDesc.bufferSize	= indices.size() * sizeof(uint32_t);
		ibDesc.bufferType	= BufferType::IndexBuffer;
		ibDesc.bufferUsage	= BufferUsage::Default;
		indexBuffer = factory.createBuffer(ibDesc, indices.data());

		vbLayout.add(VertexElementType::position2D).add(VertexElementType::texture2D);

		BufferDescriptor vbDesc{};
		vbDesc.elementSize	= vbLayout.size();
		//vbDesc.bufferSize	= ;
		vbDesc.bufferType	= BufferType::VertexBuffer;
		vbDesc.bufferUsage	= BufferUsage::Default;

		//Widgets
		{
			//From top left
			VertexBufferData bufferData{ vbLayout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f, 0.0f }, mth::vec2f{ 0.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f, -1.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, -1.0f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, 0.0f }, mth::vec2f{ 1.0f, 0.0f });

			vbDesc.bufferSize = bufferData.sizeBytes();
			widgetVB = factory.createBuffer(vbDesc, bufferData.data());
		}

		//Text
		{
			//From bottom left
			VertexBufferData bufferData{ vbLayout };
			bufferData.emplaceBack(mth::vec2f{ 0.0f, 0.0f }, mth::vec2f{ 0.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, 0.0f }, mth::vec2f{ 1.0f, 1.0f });
			bufferData.emplaceBack(mth::vec2f{ 1.0f, 1.0f }, mth::vec2f{ 1.0f, 0.0f });
			bufferData.emplaceBack(mth::vec2f{ 0.0f, 1.0f }, mth::vec2f{ 0.0f, 0.0f });

			vbDesc.bufferSize = bufferData.sizeBytes();
			textVB = factory.createBuffer(vbDesc, bufferData.data());
		}
	}

	Renderer2D::Renderer2D(Renderer2D&& other) noexcept = default;

	Renderer2D& Renderer2D::operator=(Renderer2D&& other) noexcept = default;

	Renderer2D::~Renderer2D() = default;

	void Renderer2D::begin() {
		scene.spritesToRender.clear();
		scene.textToRender.clear();
		scene.widgetsToRender.clear();
	}

	void Renderer2D::submitWidget(const std::shared_ptr<Sprite>& widget) {
		scene.widgetsToRender.push_back(widget);
	}

	void Renderer2D::submitText(const std::shared_ptr<Sprite>& text) {
		scene.textToRender.push_back(text);
	}

	void Renderer2D::end() {
		CLV_PROFILE_FUNCTION();

		//NOTE: Currently relying on the 3D renderer to clear the target

		const uint32_t vbStride = static_cast<uint32_t>(vbLayout.size());

		commandBuffer->beginEncoding(renderTarget);

		commandBuffer->setViewport({ 0, 0, screenSize.x, screenSize.y });
		commandBuffer->setDepthEnabled(false);

		commandBuffer->bindPipelineObject(*defaultPipelineObject);

		//Widgets
		for(auto& sprite : scene.widgetsToRender) {
			sprite->getMaterialInstance().bind(*commandBuffer);

			commandBuffer->bindVertexBuffer(*widgetVB, vbStride);
			commandBuffer->bindIndexBuffer(*indexBuffer);

			commandBuffer->drawIndexed(6u); //TODO: Remove hard coded value
		}

		commandBuffer->bindPipelineObject(*textPipelineObject);

		//Text
		for(auto& text : scene.textToRender) {
			text->getMaterialInstance().bind(*commandBuffer);

			commandBuffer->bindVertexBuffer(*textVB, vbStride);
			commandBuffer->bindIndexBuffer(*indexBuffer);

			commandBuffer->drawIndexed(6u); //TODO: Remove hard coded value
		}

		commandBuffer->endEncoding();
	}
}