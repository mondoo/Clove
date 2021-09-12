namespace clove {
    void RgResource::addWritePass(RgPassIdType pass) {
        writePasses.emplace(pass);
    }

    void RgResource::addReadPass(RgPassIdType pass) {
        readPasses.emplace(pass);
    }

    std::unordered_set<RgPassIdType> const &RgResource::getWritePasses() const {
        return writePasses;
    }

    std::unordered_set<RgPassIdType> const &RgResource::getReadPasses() const {
        return readPasses;
    }
}