#include "Bulb/Rendering/ForwardRenderer3D.hpp"

#include <Clove/Platform/Window.hpp>

namespace blb::rnd {
	ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API){
		graphicsFactory = std::make_shared<clv::gfx::vk::VKGraphicsFactory>(window.getNativeWindow());
	}

	void ForwardRenderer3D::begin() {
	}

	void ForwardRenderer3D::submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) {
	}

	void ForwardRenderer3D::submitLight(const DirectionalLight& light) {
	}

	void ForwardRenderer3D::submitLight(const PointLight& light) {
	}

	void ForwardRenderer3D::submitCamera(const ComposedCameraData& camera) {
	}

	void ForwardRenderer3D::submitWidget(const std::shared_ptr<Sprite>& widget) {
	}

	void ForwardRenderer3D::submitText(const std::shared_ptr<Sprite>& text) {
	}

	void ForwardRenderer3D::end() {
	}
}