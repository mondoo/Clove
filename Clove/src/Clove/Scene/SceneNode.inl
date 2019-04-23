namespace clv{
	namespace scene{
		void SceneNode::setPosition(const math::Vector3f& inPosition){
			localPosition = inPosition;
		}

		inline const math::Vector3f& SceneNode::getPosition() const{
			return localPosition;
		}

		void SceneNode::setRotation(const std::pair<math::Vector3f, float>& inRotation){
			localRotation = inRotation;
		}

		inline const std::pair<math::Vector3f, float>& SceneNode::getRotation() const{
			return localRotation;
		}

		void SceneNode::setScale(const math::Vector3f& inScale){
			localScale = inScale;
		}

		inline const math::Vector3f& SceneNode::getScale() const{
			return localScale;
		}

		inline std::vector<std::shared_ptr<SceneNode>>::const_iterator SceneNode::GetChildBeginIterator() const{
			return children.begin();
		}

		inline std::vector<std::shared_ptr<SceneNode>>::const_iterator SceneNode::GetChildEndIterator() const{
			return children.end();
		}
	}
}