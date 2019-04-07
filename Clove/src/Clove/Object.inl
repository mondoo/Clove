namespace clv{
	inline const math::Vector3f& Object::getPosition() const{
		return position;
	}

	inline std::pair<math::Vector3f, float> Object::getRotation() const{
		return std::pair<math::Vector3f, float>(rotation, rotationAngle);
	}

	inline const math::Vector3f& Object::getScale() const{
		return scale;
	}
}