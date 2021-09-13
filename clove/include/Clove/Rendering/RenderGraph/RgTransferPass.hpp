#pragma once

#include "Clove/Rendering/RenderGraph/RgPass.hpp"

#include <vector>

namespace clove {
    class RgTransferPass : public RgPass {
        //TYPES
    public:
        struct BufferWrite {
            RgResourceId bufferId{};
            std::vector<std::byte> data;
            size_t offset{};
            size_t size;
        };

        //VARIABLES
    private:
        BufferWrite writePass{};

        //FUNCTIONS
    public:
        RgTransferPass() = delete;
        RgTransferPass(RgPassId id, BufferWrite writePass);

        RgTransferPass(RgTransferPass const &other) = delete;
        RgTransferPass(RgTransferPass &&other) noexcept;

        RgTransferPass &operator=(RgTransferPass const &other) = delete;
        RgTransferPass &operator=(RgTransferPass &&other) noexcept;

        ~RgTransferPass();

        std::unordered_set<RgResourceId> getInputResources() const override;
        std::unordered_set<RgResourceId> getOutputResources() const override;

        inline BufferWrite const &getWriteOperation() const;
    };
}

#include "RgTransferPass.inl"