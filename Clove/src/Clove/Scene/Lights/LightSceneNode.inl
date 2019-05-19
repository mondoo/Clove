namespace clv{
	namespace scene{
		void LightSceneNode::setAmbientColour(const math::Vector3f& colour){
			ambientColour = colour;
		}

		inline const math::Vector3f& LightSceneNode::getAmbientColour() const{
			return ambientColour;
		}

		inline void LightSceneNode::setDiffuseColour(const math::Vector3f& colour){
			diffuseColour = colour;
		}

		inline const math::Vector3f& LightSceneNode::getDiffuseColour() const{
			return diffuseColour;
		}

		inline void LightSceneNode::setSpecularColour(const math::Vector3f& colour){
			specularColour = colour;
		}

		inline const math::Vector3f& LightSceneNode::getSpecularColour() const{
			return specularColour;
		}
	}
}