#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	class Bindable;

	struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct MaterialData{
		alignas(16) float sininess;
	};

	struct SubmitData{ //TODO: Should I just make a Drawable or something that the RenderableComponents hold?
		unsigned int indexCount = 0;
		math::Matrix4f modelData{};
		std::vector<std::unique_ptr<Bindable>>& bindables;
	};

	class Renderer{
		//VARIABLES
	protected:
		static std::unique_ptr<gfx::ShaderBufferObject<VertexData>> vertCB;
		static VertexData vData;

		static std::unique_ptr<gfx::ShaderBufferObject<MaterialData>> materialCB;
		static MaterialData mData;

		//FUNCTIONS
	public:
		static void initialise();

		static void beginScene();
		static void endScene();

		static void submitMesh(SubmitData data);
	};
}