#pragma once

namespace clv{
	class Mesh;
	class Renderer;

	namespace scene{
		class SceneNode : public std::enable_shared_from_this<SceneNode>{
			//VARIABLES
		private:
			std::weak_ptr<SceneNode> parent;
			std::vector<std::shared_ptr<SceneNode>> children;

			math::Vector3f						localPosition	= math::Vector3f(0.0f);
			std::pair<math::Vector3f, float>	localRotation	= std::pair<math::Vector3f, float>(math::Vector3f(1.0f), 0.0f);
			math::Vector3f						localScale		= math::Vector3f(1.0f);

			//FUNCTIONS
		public:
			CLV_API SceneNode();
			CLV_API SceneNode(const SceneNode& other);
			CLV_API SceneNode(SceneNode&& other) noexcept;

			CLV_API virtual ~SceneNode();

			CLV_API virtual void update(float deltaSeconds);

			CLV_API void addChild(std::shared_ptr<SceneNode> child);

			CLV_API inline void setPosition(const math::Vector3f& inPosition);
			CLV_API inline const math::Vector3f& getPosition() const;

			CLV_API inline void setRotation(const std::pair<math::Vector3f, float>& inRotation);
			CLV_API inline const std::pair<math::Vector3f, float>& getRotation() const;

			CLV_API inline void setScale(const math::Vector3f& inScale);
			CLV_API inline const math::Vector3f& getScale() const;

			CLV_API math::Matrix4f getWorldTransform();

			CLV_API inline std::vector<std::shared_ptr<SceneNode>>::const_iterator GetChildBeginIterator() const;
			CLV_API inline std::vector<std::shared_ptr<SceneNode>>::const_iterator GetChildEndIterator() const;

			CLV_API SceneNode& operator=(const SceneNode& other);
			CLV_API SceneNode& operator=(SceneNode&& other) noexcept;

		};
	}
}

#include "SceneNode.inl"