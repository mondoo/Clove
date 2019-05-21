namespace clv::scene{
	inline const math::Vector3f& Camera::getFront() const{
		return cameraFront;
	}

	inline const math::Vector3f& Camera::getUp() const{
		return cameraUp;
	}

	inline math::Vector3f Camera::getRight() const{
		return math::normalise(math::cross(cameraFront, cameraUp));
	}
}