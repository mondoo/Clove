namespace garlic::inline stem {
    std::array<clv::mth::mat4f, MAX_JOINTS> AnimatedModel::update(clv::utl::DeltaTime const deltaTime) {
        return animator.update(deltaTime);
    }
}