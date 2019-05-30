namespace clv{
	namespace scene{
		void DirectionalLight::setDirection(const math::Vector3f& direction){
			this->direction = direction;
		}

		inline const math::Vector3f& DirectionalLight::getDirection() const{
			return direction;
		}
	}
}