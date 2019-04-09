#pragma once

namespace clv{
	class Mesh;
	class Renderer;
	
	class Object{
		//VARIABLES
	private:
		math::Vector3f position	= math::Vector3f(0.0f);
		math::Vector3f rotation	= math::Vector3f(1.0f);
		math::Vector3f scale	= math::Vector3f(1.0f);
		float rotationAngle = 0.0f;

		std::unique_ptr<Mesh> objectMesh;

		//FUNCTIONS
	public:
		CLV_API Object();
		CLV_API Object(const Mesh& mesh);
		CLV_API Object(Mesh&& mesh);
		CLV_API Object(const Object& other);
		CLV_API Object(Object&& other) noexcept;

		CLV_API ~Object();

		CLV_API virtual void update(float deltaTime);
		CLV_API void draw(const Renderer& renderer, const math::Matrix4f& view, const math::Matrix4f& projection);

		CLV_API void setPosition(const math::Vector3f& newPosition);
		CLV_API inline const math::Vector3f& getPosition() const;

		CLV_API void setRotation(const math::Vector3f& newRotationAxis, float newRotationAngle);
		CLV_API inline std::pair<math::Vector3f, float> getRotation() const;

		CLV_API void setScale(const math::Vector3f& newScale);
		CLV_API inline const math::Vector3f& getScale() const;

		CLV_API Object& operator=(const Object& other);
		CLV_API Object& operator=(Object&& other) noexcept;

	private:
		math::Matrix4f computeMatrix();
	};
}

#include "Object.inl"