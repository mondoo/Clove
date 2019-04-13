#include "clvpch.hpp"
#include "Object.hpp"

#include "Clove/Mesh.hpp"
#include "Clove/Window.hpp"
#include "Clove/Rendering/Renderer.hpp"

namespace clv{
	Object::Object() = default;

	Object::Object(const Object& other) = default;

	Object::Object(Object&& other) noexcept = default;

	Object::~Object() = default;

	void Object::draw(Window& window){
		const math::Matrix4f model		= computeMatrix();
		const math::Matrix4f view		= window.getCurrentCamera().getLookAt();
		const math::Matrix4f projection = window.getCurrentCamera().getProjection();

		objectMesh->setMVP(model, view, projection);

		window.getRenderer().submit(objectMesh);
	}

	void Object::setMesh(std::shared_ptr<Mesh> inMesh){
		objectMesh = inMesh;
	}

	void Object::setPosition(const math::Vector3f& newPosition){
		position = newPosition;
	}

	void Object::setRotation(const math::Vector3f& newRotationAxis, float newRotationAngle){
		rotation = newRotationAxis;
		rotationAngle = newRotationAngle;
	}

	void Object::setScale(const math::Vector3f& newScale){
		scale = newScale;
	}

	Object& Object::operator=(const Object& other) = default;

	Object& Object::operator=(Object&& other) noexcept = default;

	math::Matrix4f Object::computeMatrix(){
		const math::Matrix4f identity(1.0f);

		const math::Matrix4f transformMatrix	= math::translate<float, math::qualifier::defaultp>(identity, position);
		const math::Matrix4f rotationMatrix		= math::rotate<float, math::qualifier::defaultp>(identity, math::asRadians(rotationAngle), rotation);
		const math::Matrix4f scaleMatrix		= math::scale<float, math::qualifier::defaultp>(identity, scale);

		return transformMatrix * rotationMatrix * scaleMatrix;
	}
}