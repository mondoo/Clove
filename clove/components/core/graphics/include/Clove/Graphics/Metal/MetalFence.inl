namespace garlic::clove {
	dispatch_semaphore_t MetalFence::getSemaphore() const {
		return semaphore;
	}
}
