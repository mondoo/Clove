namespace clv{
	namespace scene{
		inline const math::Vector3f& CameraSceneNode::getFront() const{
			return cameraFront;
		}

		inline const math::Vector3f& CameraSceneNode::getUp() const{
			return cameraUp;
		}

		inline math::Vector3f CameraSceneNode::getRight() const{
			return math::normalise(math::cross(cameraFront, cameraUp));
		}
	}
}