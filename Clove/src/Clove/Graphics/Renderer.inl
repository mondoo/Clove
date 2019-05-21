namespace clv::gfx{
	inline const scene::Camera& Renderer::getActiveCamera() const{
		CLV_ASSERT(activeCamera != nullptr, "No active camera set!");
		return *activeCamera;
	}
}