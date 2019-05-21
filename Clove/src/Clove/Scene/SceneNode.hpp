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
			SceneNode();
			SceneNode(const SceneNode& other);
			SceneNode(SceneNode&& other) noexcept;
			SceneNode& operator=(const SceneNode& other);
			SceneNode& operator=(SceneNode&& other) noexcept;
			virtual ~SceneNode();

			virtual void update(float deltaSeconds);

			void addChild(const std::shared_ptr<SceneNode>& child);

			inline void setPosition(const math::Vector3f& inPosition);
			inline const math::Vector3f& getPosition() const;

			inline void setRotation(const std::pair<math::Vector3f, float>& inRotation);
			inline const std::pair<math::Vector3f, float>& getRotation() const;

			inline void setScale(const math::Vector3f& inScale);
			inline const math::Vector3f& getScale() const;

			math::Matrix4f getWorldTransform() const;

			inline std::vector<std::shared_ptr<SceneNode>>::const_iterator GetChildBeginIterator() const;
			inline std::vector<std::shared_ptr<SceneNode>>::const_iterator GetChildEndIterator() const;
		};
	}
}

#include "SceneNode.inl"