namespace clv{
	inline const glm::vec3& Object::getPosition() const{
		return position;
	}

	inline std::pair<glm::vec3, float> Object::getRotation() const{
		return std::pair<glm::vec3, float>(rotation, rotationAngle);
	}

	inline const glm::vec3& clv::Object::getScale() const{
		return scale;
	}
}