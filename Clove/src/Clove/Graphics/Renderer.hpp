#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	class Bindable;
	class IndexBuffer;
	enum class API;

	//TODO: Temp putting these here
	struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct MaterialData{
		alignas(16) float sininess;
	};
	//

	//Submit something like this?
	struct SubmitData{
		unsigned int indexCount = 0;
		math::Matrix4f modelData{};
		std::vector<std::unique_ptr<Bindable>>& bindables;
	};

	//Explore this - then figure out how the bindables will bind and create themselves

	class Renderer{
		//VARIABLES
	protected:
		//static constexpr math::Vector4f clearColor = { 1.01f, 0.5f, 0.5f, 1.0f };

		//TODO: Temp putting these here
		static std::unique_ptr<gfx::ShaderBufferObject<VertexData>> vertCB;
		static VertexData vData;

		static std::unique_ptr<gfx::ShaderBufferObject<MaterialData>> materialCB;
		static MaterialData mData;
		//

		//should this hold the context?

		//FUNCTIONS
	public:

		//TODO: Does this still recieve the context?
		//	-The only reason the api recieves it is so DX can grab it's boys

		/*static std::unique_ptr<Renderer> createRenderer(const Context& context);

		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;*/

		static void beginScene();
		static void endScene();

		//TODO: Need better parameters - could just pass the renderable component here?
		//static void submitMesh(const math::Matrix4f& model, IndexBuffer* IB, const std::vector<std::unique_ptr<Bindable>>& bindables);
		static void submitMesh(SubmitData data);
	};
}