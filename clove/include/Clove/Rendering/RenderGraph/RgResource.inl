namespace garlic::clove {
    void RgResource::addWritePass(PassIdType pass) {
        writePasses.emplace(pass);
    }

    void RgResource::addReadPass(PassIdType pass) {
        readPasses.emplace(pass);
    }

    ResourceIdType RgResource::getId() const {
        return id;
    }

    std::unordered_set<PassIdType> const &RgResource::getWritePasses() const {
        return writePasses;
    }

    std::unordered_set<PassIdType> const &RgResource::getReadPasses() const {
        return readPasses;
    }
}