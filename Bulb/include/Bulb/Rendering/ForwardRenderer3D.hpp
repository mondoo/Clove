#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>

namespace clv::gfx {
	class RenderTarget;
}

namespace blb::rnd {
	class Sprite;
	class Mesh;
}

namespace blb::rnd {
	class Renderer3D {
		//TYPES
	public:
		struct ComposedCameraData {
			clv::gfx::Viewport viewport;
			CameraRenderData bufferData;
			std::shared_ptr<clv::gfx::RenderTarget> target;
		};

		//FUNCTIONS
	public:
		virtual void begin() = 0;

		virtual void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) = 0;
		virtual void submitLight(const DirectionalLight& light) = 0;
		virtual void submitLight(const PointLight& light) = 0;
		virtual void submitCamera(const ComposedCameraData& camera) = 0;

		virtual void submitWidget(const std::shared_ptr<Sprite>& widget) = 0;
		virtual void submitText(const std::shared_ptr<Sprite>& text) = 0;

		virtual void end() = 0;
	};
}

namespace blb::rnd {
	class ForwardRenderer3D : public Renderer3D {
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		//TODO: Ctors

		void begin() final;

		void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) final;
		void submitLight(const DirectionalLight& light) final;
		void submitLight(const PointLight& light) final;
		void submitCamera(const ComposedCameraData& camera) final;

		void submitWidget(const std::shared_ptr<Sprite>& widget) final;
		void submitText(const std::shared_ptr<Sprite>& text) final;

		void end() final;
	};
}