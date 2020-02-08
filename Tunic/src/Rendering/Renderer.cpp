#include "Tunic/Rendering/Renderer.hpp"

#include "Tunic/Rendering/RenderingConstants.hpp"

#include <Clove/Graphics/Core/GraphicsFactory.hpp>
#include <Clove/Graphics/Core/CommandBuffer.hpp>
#include <Clove/Graphics/Core/Surface.hpp>
#include <Clove/Graphics/Core/RenderTarget.hpp>
#include <Clove/Graphics/Core/PipelineObject.hpp>
#include <Clove/Graphics/Core/Resources/Texture.hpp>

using namespace clv;

namespace tnc::rnd{
	Renderer::Renderer(gfx::GraphicsFactory& factory, std::shared_ptr<gfx::Surface> surface)
		: surface(std::move(surface)){
		
		//Shadow map
		gfx::TextureDescriptor tdesc{};
		tdesc.style			= gfx::TextureStyle::Cubemap;
		tdesc.usage			= gfx::TextureUsage::RenderTarget_Depth;
		tdesc.dimensions	= { shadowMapSize, shadowMapSize };
		tdesc.arraySize		= MAX_LIGHTS;

		shadowMapTexture	= factory.createTexture(tdesc, nullptr, 4);
		shadowRenderTarget	= factory.createRenderTarget(nullptr, shadowMapTexture.get());
		shadowCommandBuffer = factory.createCommandBuffer();

		auto shadowVS = factory.createShader({ gfx::ShaderStage::Vertex }, "res/Shaders/CubeShadowMap-vs.hlsl");
		auto shadowGS = factory.createShader({ gfx::ShaderStage::Geometry }, "res/Shaders/CubeShadowMap-gs.hlsl");
		auto shadowPS = factory.createShader({ gfx::ShaderStage::Pixel }, "res/Shaders/CubeShadowMap-ps.hlsl");
		shadowPipelineObject = factory.createPipelineObject();
		shadowPipelineObject->setVertexShader(*shadowVS);
		shadowPipelineObject->setGeometryShader(*shadowGS);
		shadowPipelineObject->setPixelShader(*shadowPS);

		//Mesh
		meshCommandBuffer = factory.createCommandBuffer();

		auto meshVS = factory.createShader({ gfx::ShaderStage::Vertex }, "res/Shaders/Lit-vs.hlsl");
		auto meshPS = factory.createShader({ gfx::ShaderStage::Pixel }, "res/Shaders/Lit-ps.hlsl");
		meshPipelineObject = factory.createPipelineObject();
		meshPipelineObject->setVertexShader(*meshVS);
		meshPipelineObject->setPixelShader(*meshPS);

		//Sprites
		spriteCommandBuffer = factory.createCommandBuffer();

		auto spriteVS = factory.createShader({ gfx::ShaderStage::Vertex }, "res/Shaders/2D-vs.hlsl");
		auto spritePS = factory.createShader({ gfx::ShaderStage::Pixel }, "res/Shaders/2D-ps.hlsl");
		spritePipelineObject = factory.createPipelineObject();
		spritePipelineObject->setVertexShader(*spriteVS);
		spritePipelineObject->setPixelShader(*spritePS);

		//Fonts
		//TODO:
	}

	void Renderer::begin(){
		shadowCommandBuffer->beginEncoding(shadowRenderTarget);
		meshCommandBuffer->beginEncoding(surface->getRenderTarget());
		spriteCommandBuffer->beginEncoding(surface->getRenderTarget());

		meshCommandBuffer->clearTarget(); //Sprite is cleared here
		shadowCommandBuffer->clearTarget();
	}

	void Renderer::submitMesh(const Mesh& mesh){

	}

	void Renderer::submitSprite(const Sprite& sprite){

	}

	void Renderer::end(){
		shadowCommandBuffer->endEncoding();
		meshCommandBuffer->endEncoding();
		spriteCommandBuffer->endEncoding();
	}
}