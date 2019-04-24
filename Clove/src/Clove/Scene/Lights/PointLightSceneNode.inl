namespace clv{
	namespace scene{
		inline float PointLightSceneNode::getConstant() const{
			return constant;
		}

		void PointLightSceneNode::setLinear(float linear){
			this->linear = linear;
		}

		inline float PointLightSceneNode::getLinear() const{
			return linear;
		}

		inline void PointLightSceneNode::setQuadratic(float quadratic){
			this->quadratic = quadratic;
		}

		inline float PointLightSceneNode::getQuadratic() const{
			return quadratic;
		}
	}
}