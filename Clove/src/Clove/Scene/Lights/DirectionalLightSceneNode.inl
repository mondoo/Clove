namespace clv{
	namespace scene{
		void DirectionalLightSceneNode::setDirection(const math::Vector3f& direction){
			this->direction = direction;
		}

		inline const math::Vector3f& DirectionalLightSceneNode::getDirection() const{
			return direction;
		}
	}
}