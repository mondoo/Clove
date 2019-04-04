#include "clvpch.h"
#include "Object.h"
#include "Clove/Mesh.h"
#include "Clove/Rendering/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

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
		//Implementation needed?
	}

	void Object::draw(const Renderer& renderer, const glm::mat4& view, const glm::mat4& projection){
		if(objectMesh){
			const glm::mat4 matrix = computeMatrix();
			objectMesh->setMVP(matrix, view, projection);
			objectMesh->draw(renderer);
		}
	}

	void Object::setPosition(const glm::vec3& newPosition){
		position = newPosition;
	}

	void Object::setRotation(const glm::vec3& newRotationAxis, float newRotationAngle){
		rotation = newRotationAxis;
		rotationAngle = newRotationAngle;
	}

	void Object::setScale(const glm::vec3& newScale){
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

	glm::mat4 Object::computeMatrix(){
		const glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), position);
		const glm::mat4 rotationMatrix	= glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotation);
		const glm::mat4 scaleMatrix		= glm::scale(glm::mat4(1.0f), scale);

		return transformMatrix * rotationMatrix * scaleMatrix;
	}
}