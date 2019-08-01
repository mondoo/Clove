#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/RenderTarget.hpp"

namespace clv::gfx{
	void MeshRenderData::bind() const{
		vertexBuffer->bind();
		indexBuffer->bind();
		shader->bind();
		diffTexture->bind();
		if(specTexture){ //Spec can be nullptr
			specTexture->bind();
		}
	}

	void SpriteRenderData::bind() const{
		texture->bind();
	}

	std::shared_ptr<gfx::ShaderBufferObject<VertexData>> Renderer::vertSBO;
	std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> Renderer::materialSBO;
	std::shared_ptr<gfx::ShaderBufferObject<SpriteShaderData>> Renderer::spriteSBO;

	std::shared_ptr<gfx::ShaderBufferObject<ViewData>> Renderer::viewDataSBO;
	std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> Renderer::viewPosSBO;

	std::shared_ptr<gfx::ShaderBufferObject<LightData>> Renderer::lightDataSBO;

	std::vector<MeshRenderData> Renderer::meshSubmissionData;
	std::vector<SpriteRenderData> Renderer::spriteSubmissionData;
	CameraRenderData Renderer::cameraSubmissionData;

	std::shared_ptr<VertexBuffer> Renderer::spriteVBBuffer;
	std::shared_ptr<IndexBuffer> Renderer::spriteIBBuffer;
	std::shared_ptr<Shader> Renderer::spriteShader;
	math::Matrix4f Renderer::spriteProj = {};

	std::shared_ptr<RenderTarget> Renderer::renderTarget;
	std::shared_ptr<VertexBuffer> Renderer::renderTargetVB;
	std::shared_ptr<IndexBuffer> Renderer::renderTargetIB;
	std::shared_ptr<Shader> Renderer::renderTargetShader;
	std::shared_ptr<Texture> Renderer::renderTargetTexture;

	void Renderer::initialise(){
		vertSBO = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderType::Vertex, gfx::BBP_ModelData);
		materialSBO = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderType::Pixel, gfx::BBP_MaterialData);
		spriteSBO = gfx::BindableFactory::createShaderBufferObject<SpriteShaderData>(gfx::ShaderType::Vertex, gfx::BBP_2DData);

		viewDataSBO = gfx::BindableFactory::createShaderBufferObject<ViewData>(gfx::ShaderType::Vertex, gfx::BBP_CameraMatrices);
		viewPosSBO = gfx::BindableFactory::createShaderBufferObject<ViewPos>(gfx::ShaderType::Pixel, gfx::BBP_ViewData);
		
		lightDataSBO = gfx::BindableFactory::createShaderBufferObject<LightData>(gfx::ShaderType::Pixel, gfx::BBP_PointLightData);

		vertSBO->bind();
		materialSBO->bind();
		spriteSBO->bind();

		viewDataSBO->bind();
		viewPosSBO->bind();

		lightDataSBO->bind();

		materialSBO->update({ 32.0f });

		RenderCommand::setBlendState(true);

		//QUAD STUFF
		//Shader
		spriteShader = gfx::BindableFactory::createShader();
		spriteShader->attachShader(gfx::ShaderType::Vertex2D);
		spriteShader->attachShader(gfx::ShaderType::Pixel2D);

		//VB
		gfx::VertexLayout layout;
		layout.add(gfx::VertexElementType::position2D).add(gfx::VertexElementType::texture2D);
		gfx::VertexBufferData bufferData(std::move(layout));
		bufferData.emplaceBack(math::Vector2f{-0.5f, -0.5f }, math::Vector2f{ 0.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{ 0.5f, -0.5f }, math::Vector2f{ 1.0f, 0.0f });
		bufferData.emplaceBack(math::Vector2f{-0.5f,  0.5f }, math::Vector2f{ 0.0f, 1.0f });
		bufferData.emplaceBack(math::Vector2f{ 0.5f,  0.5f }, math::Vector2f{ 1.0f, 1.0f });

		spriteVBBuffer = gfx::BindableFactory::createVertexBuffer(bufferData, *spriteShader);

		//IB
		std::vector<unsigned int> indices = {
			1, 3, 0,
			3, 2, 0
		};
		spriteIBBuffer = gfx::BindableFactory::createIndexBuffer(indices);

		//Proj
		const float halfWidth = static_cast<float>(Application::get().getWindow().getWidth()) / 2;
		const float halfHeight = static_cast<float>(Application::get().getWindow().getHeight()) / 2;

		spriteProj = math::createOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight);

		//FRAME BUFFER STUFF
		//Shader
		renderTargetShader = BindableFactory::createShader();
		renderTargetShader->attachShader(ShaderType::VertexFB);
		renderTargetShader->attachShader(ShaderType::PixelFB);

		//VB
		VertexLayout fblayout;
		fblayout.add(VertexElementType::position2D).add(VertexElementType::texture2D);
		VertexBufferData fbbufferData(std::move(fblayout));
		fbbufferData.emplaceBack(math::Vector2f{-1.0f, -1.0f }, math::Vector2f{ 0.0f, 0.0f });
		fbbufferData.emplaceBack(math::Vector2f{ 1.0f, -1.0f }, math::Vector2f{ 1.0f, 0.0f });
		fbbufferData.emplaceBack(math::Vector2f{-1.0f,  1.0f }, math::Vector2f{ 0.0f, 1.0f });
		fbbufferData.emplaceBack(math::Vector2f{ 1.0f,  1.0f }, math::Vector2f{ 1.0f, 1.0f });

		renderTargetVB = BindableFactory::createVertexBuffer(fbbufferData, *renderTargetShader);

		//IB
		renderTargetIB = BindableFactory::createIndexBuffer(indices);

		//Textures
		renderTargetTexture = BindableFactory::createTexture(Application::get().getWindow().getWidth(), Application::get().getWindow().getHeight(), TextureUsage::RenderTarget, TBP_FrameBuffer);
		
		renderTarget = RenderTarget::createRenderTarget(*renderTargetTexture);
	}

	void Renderer::beginScene(){
		
	}

	void Renderer::endScene(){
		//MESH
		//First pass
		RenderCommand::setRenderTarget(*renderTarget);
		RenderCommand::clear(); //make sure it's clean
		RenderCommand::setDepthBuffer(true);

		for(auto& data : meshSubmissionData){
			vertSBO->update({ data.modelData, math::transpose(math::inverse(data.modelData)) });
			data.bind();
			RenderCommand::drawIndexed(data.indexBuffer->getIndexCount());
		}

		meshSubmissionData.clear();

		//Second pass
		RenderCommand::resetRenderTarget();
		RenderCommand::setDepthBuffer(false);
		
		renderTargetVB->bind();
		renderTargetIB->bind();
		renderTargetShader->bind();
		renderTargetTexture->bind();
		
		RenderCommand::drawIndexed(renderTargetIB->getIndexCount());

		//SPRITE
		spriteVBBuffer->bind();
		spriteIBBuffer->bind();
		spriteShader->bind();
		spriteSBO->bind();

		for(auto& data : spriteSubmissionData){
			spriteSBO->update({ spriteProj * data.modelData });
			data.bind();
			RenderCommand::drawIndexed(spriteIBBuffer->getIndexCount());
		}

		spriteSubmissionData.clear();
	}

	void Renderer::submitMesh(const MeshRenderData& data){
		meshSubmissionData.emplace_back(data);
	}

	void Renderer::submitSprite(const SpriteRenderData& data){
		spriteSubmissionData.emplace_back(data);
	}

	void Renderer::setCamera(const CameraRenderData& data){
		viewDataSBO->update({ data.lookAt, data.projection });
		viewPosSBO->update({ data.position });
	}

	void Renderer::submitLight(const LightData& data){
		lightDataSBO->update(data);
	}
}