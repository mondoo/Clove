#pragma once

#include "Tunic/ECS/Core/System.hpp"

#include "Tunic/Rendering/MaterialInstance.hpp"
#include "Clove/Graphics/Core/GraphicsConstants.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/ShaderBufferTypes.hpp"

namespace clv::gfx{
	class RenderTarget;
	class Texture;
	class PipelineObject;
	class CommandBuffer;
}

namespace tnc::rnd{
	class Mesh;
}

namespace tnc::ecs::_3D{
	class RenderSystem : public System{
		//TYPES
	private:
		struct ComposedCameraData{
			clv::gfx::Viewport viewport;
			clv::gfx::CameraRenderData bufferData;
			std::shared_ptr<clv::gfx::RenderTarget> target;
		};

		struct SceneData{
			//VARIABLES
		public:
			std::vector<ComposedCameraData> cameras;

			std::vector<std::shared_ptr<rnd::Mesh>> meshesToRender;

			clv::gfx::PointLightShaderData currentLightInfo;
			clv::gfx::PointShadowDepthData currentShadowDepth;
			clv::uint32 numLights = 0;
			std::array<std::array<clv::mth::mat4f, 6>, clv::gfx::MAX_LIGHTS> shadowTransforms = {};

			//FUNCTIONS
		public:
			SceneData();

			~SceneData();

			//TODO: .inl
			template<typename FunctionPointer>
			void forEachMesh(FunctionPointer&& function){
				std::for_each(meshesToRender.begin(), meshesToRender.end(), function);
			}
		};

		//VARIABLES
	private:
		SceneData sceneData;

		std::shared_ptr<clv::gfx::CommandBuffer> windowCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> shadowMapCommandBuffer;
		std::shared_ptr<clv::gfx::Texture> shadowMapTexture;
		rnd::MaterialInstance cubeShadowMaterial;

		std::shared_ptr<clv::gfx::PipelineObject> defaultPipeline;
		std::shared_ptr<clv::gfx::PipelineObject> shadowPipeline;

		//FUNCTIONS
	public:
		RenderSystem();

		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;

		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;

		virtual ~RenderSystem();

		virtual void preUpdate() override;
		virtual void update(clv::utl::DeltaTime deltaTime) override;
		virtual void postUpdate() override;
	};
}
