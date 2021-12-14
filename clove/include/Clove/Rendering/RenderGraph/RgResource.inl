namespace clove {
    void RgResource::addWritePass(RgPassId pass) {
        writePasses.emplace(pass);
    }

    void RgResource::addReadPass(RgPassId pass) {
        readPasses.emplace(pass);
    }

    std::unordered_set<RgPassId> const &RgResource::getWritePasses() const {
        return writePasses;
    }

    std::unordered_set<RgPassId> const &RgResource::getReadPasses() const {
        return readPasses;
    }
}