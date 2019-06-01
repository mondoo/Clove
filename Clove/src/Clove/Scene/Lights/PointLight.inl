namespace clv{
	namespace scene{
		inline float PointLight::getConstant() const{
			return constant;
		}

		void PointLight::setLinear(float linear){
			this->linear = linear;
		}

		inline float PointLight::getLinear() const{
			return linear;
		}

		inline void PointLight::setQuadratic(float quadratic){
			this->quadratic = quadratic;
		}

		inline float PointLight::getQuadratic() const{
			return quadratic;
		}
	}
}