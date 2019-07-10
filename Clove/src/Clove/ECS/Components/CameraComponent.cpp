#include "clvpch.hpp"
#include "CameraComponent.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::ecs{
	CameraComponent::CameraComponent(){
		setProjectionMode(ProjectionMode::perspective);

		sboMat = gfx::BindableFactory::createShaderBufferObject<ViewData>(gfx::ShaderTypes::Vertex, gfx::BBP_CameraMatrices);
		sboMat->bind();

		sboPos = gfx::BindableFactory::createShaderBufferObject<ViewPos>(gfx::ShaderTypes::Pixel, gfx::BBP_ViewData);
		sboPos->bind();
	}

	CameraComponent::CameraComponent(CameraComponent&& other) noexcept = default;

	CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept = default;

	CameraComponent::~CameraComponent() = default;

	const math::Vector3f& CameraComponent::getFront() const{
		return cameraFront;
	}

	const math::Vector3f& CameraComponent::getUp() const{
		return cameraUp;
	}

	const math::Vector3f& CameraComponent::getRight() const{
		return cameraRight;
	}

	void CameraComponent::updateFront(float pitch, float yaw){
		this->pitch = pitch;
		this->yaw = yaw;
	}

	void CameraComponent::setProjectionMode(ProjectionMode mode){
		switch(mode){
			case ProjectionMode::orthographic:
				//TODO: Need to get the window dimensions (ie. left = -(1920 / 2))
				currentProjection = math::createOrthographicMatrix(-1.0f, 1.0f, -1.0f, 1.0f);
				break;

			case ProjectionMode::perspective:
				currentProjection = math::createPerspectiveMatrix(45.0f, 16.0f / 9.0f, 0.5f, 10000.0f);
				break;

			default:
				break;
		}
	}
}