#include "clvpch.h"
#include "Object.h"
#include "Clove/Mesh.h"
#include "Clove/Rendering/Renderer.h"



namespace clv{
	Object::Object() = default;

	Object::Object(const Mesh& mesh){
		objectMesh = std::make_unique<Mesh>(mesh);
	}

	Object::Object(const Object& other){
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		rotationAngle = other.rotationAngle;

		if (other.objectMesh){
			objectMesh = std::make_unique<Mesh>(*other.objectMesh);
		} else{
			objectMesh.reset();
		}
	}

	Object::Object(Object&& other){
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		rotationAngle = other.rotationAngle;

		objectMesh = std::move(other.objectMesh);
	}

	Object::~Object() = default;

	void Object::update(float deltaTime){
		//TODO: This will probably come much much later
	}

	void Object::draw(const Renderer& renderer, const math::Matrix4f& view, const math::Matrix4f& projection){
		if(objectMesh){
			const math::Matrix4f matrix = computeMatrix();
			objectMesh->setMVP(matrix, view, projection);
			objectMesh->draw(renderer);
		}
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

	Object& Object::operator=(const Object& other){
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		rotationAngle = other.rotationAngle;

		objectMesh = std::make_unique<Mesh>(*other.objectMesh);

		return *this;
	}

	Object& Object::operator=(Object&& other){
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		rotationAngle = other.rotationAngle;

		objectMesh = std::move(other.objectMesh);

		return *this;
	}

	math::Matrix4f Object::computeMatrix(){
		const math::Matrix4f identity(1.0f);

		const math::Matrix4f transformMatrix	= math::translate<float, math::qualifier::defaultp>(identity, position);
		const math::Matrix4f rotationMatrix		= math::rotate<float, math::qualifier::defaultp>(identity, math::asRadians(rotationAngle), rotation);
		const math::Matrix4f scaleMatrix		= math::scale<float, math::qualifier::defaultp>(identity, scale);

		return transformMatrix * rotationMatrix * scaleMatrix;
	}
}