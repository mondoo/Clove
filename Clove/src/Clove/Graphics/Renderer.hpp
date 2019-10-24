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
		//FUNCTIONS
	public:
		static void initialise();
		static void shutDown();

		static void beginScene();
		static void endScene();

		static void setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget);
		static void clearRenderTargets();
		static void removeRenderTarget();

		static void submitMesh(const std::shared_ptr<Mesh>& mesh);
		static void setCamera(const CameraRenderData& data);
		static void submitPointLight(const PointLightData& data);
	};
}