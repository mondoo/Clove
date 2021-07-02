namespace garlic::clove {
    void RgRenderPass::addSubmission(Submission submission) {
        submissions.emplace_back(std::move(submission));
    }
}