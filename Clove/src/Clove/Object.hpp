#pragma once

namespace clv{
	class Mesh;
	class Window;
	
	class Object{
		//VARIABLES
	private:
		math::Vector3f position	= math::Vector3f(0.0f);
		math::Vector3f rotation	= math::Vector3f(1.0f);
		math::Vector3f scale	= math::Vector3f(1.0f);
		float rotationAngle		= 0.0f;

		std::shared_ptr<Mesh> objectMesh;

		//FUNCTIONS
	public:
		CLV_API Object();
		CLV_API Object(const Object& other);
		CLV_API Object(Object&& other) noexcept;

		CLV_API ~Object();

		CLV_API void draw(Window& window);

		CLV_API void setMesh(std::shared_ptr<Mesh> inMesh);
		CLV_API inline std::shared_ptr<Mesh> getMesh() const;
		
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