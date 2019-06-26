#include "clvpch.hpp"
#include "Render2DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs{
	Render2DSystem::Render2DSystem() = default;

	Render2DSystem::Render2DSystem(Render2DSystem&& other) noexcept = default;

	Render2DSystem& Render2DSystem::operator=(Render2DSystem&& other) noexcept = default;

	Render2DSystem::~Render2DSystem() = default;

	void Render2DSystem::update(float deltaTime){
		//TODO
	}

	void Render2DSystem::initialiseRenderer(const std::shared_ptr<gfx::Renderer>& renderer){
		this->renderer = renderer;
	}
}