namespace clove {
    SubSystem::SubSystem(std::string name)
        : debugName(std::move(name)) {
    }

    std::string const &SubSystem::getName() const {
        return debugName;
    }
}