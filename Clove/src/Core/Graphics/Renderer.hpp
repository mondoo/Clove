#pragma once

namespace clv::gfx{
	class RenderTarget;
	class Mesh;
	struct CameraRenderData;
	struct PointLightData;
}

namespace clv::gfx{
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