#pragma once

namespace clv{
	class Mesh;
	class Renderer;
	
	class Object{
		//VARIABLES
	private:
		glm::vec3 position	= glm::vec3(0.0f);
		glm::vec3 rotation	= glm::vec3(1.0f);
		glm::vec3 scale		= glm::vec3(1.0f);
		float rotationAngle = 0.0f;

		//TODO: Temp public!
	public:
		std::unique_ptr<Mesh> objectMesh;

		//FUNCTIONS
	public:
		CLV_API Object();
		CLV_API Object(const Mesh& mesh);
		CLV_API Object(const Object& other);
		CLV_API Object(Object&& other);

		CLV_API ~Object();

		CLV_API virtual void update(float deltaTime);
		CLV_API void draw(const Renderer& renderer, const glm::mat4& view, const glm::mat4& projection);

		CLV_API void setPosition(const glm::vec3& newPosition);
		CLV_API inline const glm::vec3& getPosition() const;

		CLV_API void setRotation(const glm::vec3& newRotationAxis, float newRotationAngle);
		CLV_API inline std::pair<glm::vec3, float> getRotation() const;

		CLV_API void setScale(const glm::vec3& newScale);
		CLV_API inline const glm::vec3& getScale() const;

		CLV_API Object& operator=(const Object& other);
		CLV_API Object& operator=(Object&& other);

	private:
		glm::mat4 computeMatrix();
	};
}

#include "Clove/Object.inl"