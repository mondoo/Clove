#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Clove/Graphics/GraphicsConstants.hpp"
#include "Clove/Graphics/MaterialInstance.hpp"

namespace clv::gfx{
	class RenderTarget;
	class Mesh;
	struct CameraRenderData;
	struct PointLightData;

	class Renderer{
		//VARIABLES
	public:
		//TODO: Is there a better place to encapsulate this?
		static constexpr uint32 shadowMapSize = 1024; //arbitrary shadowmap size

		//FUNCTIONS
	public:
		static void initialise();
		static void shutDown();

		static void beginScene(); //How would I begin scene with the ecs?
		static void endScene();

		static void setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget);
		static void clearRenderTargets();
		static void removeRenderTarget();

		static void submitMesh(const std::shared_ptr<Mesh>& mesh);
		static void setCamera(const CameraRenderData& data);
		static void submitPointLight(const PointLightData& data);
	};
}