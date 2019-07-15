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

	struct SpriteShaderData{
		math::Matrix4f modelProjection;
	};

	struct SubmitData{ //TODO: Should I just make a Drawable or something that the RenderableComponents hold?
		unsigned int indexCount = 0;
		math::Matrix4f modelData{};
		std::vector<std::shared_ptr<Bindable>>& bindables;
	};

	class Renderer{
		//VARIABLES
	protected:
		static std::shared_ptr<gfx::ShaderBufferObject<VertexData>> vertSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> materialSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<SpriteShaderData>> spriteSBO;

		static std::vector<SubmitData> meshSubmissionData;
		static std::vector<SubmitData> spriteSubmissionData;

		//FUNCTIONS
	public:
		static void initialise();

		static void beginScene();
		static void endScene();

		//TODO: Make const ref? relying on the invoking call to move
		static void submitMesh(SubmitData data);
		static void submitSprite(SubmitData data);
	};
}