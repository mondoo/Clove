#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>

namespace clv::plt {
	class Window;
}

namespace clv::gfx {
	class RenderTarget;
}

namespace blb::rnd {
	class Sprite;
	class Mesh;
}

namespace blb::rnd {
	class IRenderer3D { //Temp IRenderer3D until old renderer has been removed
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

#include <Clove/Graphics/Vulkan/VKGraphicsFactory.hpp>

namespace blb::rnd {
	class ForwardRenderer3D : public IRenderer3D {
		//VARIABLES
	private:
		//Using shared_ptrs of the actual types until API is abstracted
		std::shared_ptr<clv::gfx::vk::VKGraphicsFactory> graphicsFactory;

		//FUNCTIONS
	public:
		//TODO: Ctors
		ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API);

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