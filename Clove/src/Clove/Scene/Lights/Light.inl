namespace clv{
	namespace scene{
		void Light::setAmbientColour(const math::Vector3f& colour){
			ambientColour = colour;
		}

		inline const math::Vector3f& Light::getAmbientColour() const{
			return ambientColour;
		}

		inline void Light::setDiffuseColour(const math::Vector3f& colour){
			diffuseColour = colour;
		}

		inline const math::Vector3f& Light::getDiffuseColour() const{
			return diffuseColour;
		}

		inline void Light::setSpecularColour(const math::Vector3f& colour){
			specularColour = colour;
		}

		inline const math::Vector3f& Light::getSpecularColour() const{
			return specularColour;
		}
	}
}