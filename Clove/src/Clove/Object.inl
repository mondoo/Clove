namespace clv{
	inline std::shared_ptr<Mesh> Object::getMesh() const{
		return objectMesh;
	}
	
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