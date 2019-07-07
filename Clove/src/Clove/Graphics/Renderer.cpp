#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Clove/Graphics/RenderCommand.hpp"

//Keeping?
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"

namespace clv::gfx{
	Renderer::Renderer(){
		//TODO: Initialise API here?


		//TODO: Temp?
		vertCB = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderTypes::Vertex, gfx::BBP_ModelData);
		materialCB = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderTypes::Pixel, gfx::BBP_MaterialData);

		mData.sininess = 32.0f;
		materialCB->update(mData, *renderer);
		//
	}

	Renderer::~Renderer() = default;

	void Renderer::beginScene(){
		RenderCommand::clear();
	}

	void Renderer::endScene(){
		//TODO
	}

	void Renderer::submitMesh(const math::Matrix4f& model, IndexBuffer* IB, const std::vector<std::unique_ptr<Bindable>>& bindables){
		//What data am i expecting here?
		//Will this just add data to an array?
		//It will basically hold the bindables and then bind and draw them at the end of the scene?
		
		//TODO: How to get index buffer?


		
		//TODO: Temp just putting it here for now
		vertCB->bind();
		materialCB->bind();
		//

		//TODO: Temp just putting it here for now
		vData.model = model;
		vData.normalMatrix = math::transpose(math::inverse(model));
		vertCB->update(vData, *renderer);
		//

		for(const auto& bindable : bindables){
			bindable->bind();
		}

		//TODO: Need IB
		RenderCommand::drawIndexed(IB->getIndexCount());
	}

	//std::unique_ptr<Renderer> Renderer::createRenderer(const Context& context){
	//	std::unique_ptr<Renderer> renderer;
	//	
	//	switch(context.getAPI()){
	//		case API::OpenGL4:
	//			CLV_LOG_TRACE("Creating OpenGL renderer");
	//			renderer = std::make_unique<GL4Renderer>(context);
	//			break;
	//		#if CLV_PLATFORM_WINDOWS
	//		case API::DirectX11:
	//			CLV_LOG_TRACE("Creating DirectX11 renderer");
	//			renderer = std::make_unique<DX11Renderer>(context);
	//			break;
	//		#endif
	//		default:
	//			CLV_LOG_ERROR("Default statement hit. No renderer initialised: {0}", __func__);
	//			break;
	//	}

	//	if(renderer == nullptr){
	//		CLV_ASSERT(false, "Renderer failed to initialise!");
	//	} else{
	//		CLV_LOG_DEBUG("Renderer created succesfully");
	//	}

	//	return renderer;
	//}
}