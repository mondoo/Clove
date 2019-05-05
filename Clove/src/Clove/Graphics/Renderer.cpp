#include "clvpch.hpp"
#include "Clove/Graphics/Renderer.hpp"

#include "Clove/Scene/CameraSceneNode.hpp"

/*
other todos:
-remove glfw window data thing
-stop the renderables submitting themselves - just pass the scene to the renderer/window
-set up vsync here
*/

namespace clv{
	namespace graphics{
		Renderer::Renderer() = default;

		Renderer::~Renderer() = default;
	}
}