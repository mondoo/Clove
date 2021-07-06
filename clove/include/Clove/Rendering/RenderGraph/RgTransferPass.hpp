#pragma once

#include "Clove/Rendering/RenderGraph/RgPass.hpp"

namespace garlic::clove {
    class RgTransferPass : RgPass {
        //TYPES
    public:
        struct BufferWrite {
            ResourceIdType bufferId{};
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
        RgTransferPass(PassIdType id, BufferWrite writePass);

        RgTransferPass(RgTransferPass const &other) = delete;
        RgTransferPass(RgTransferPass &&other) noexcept;

        RgTransferPass &operator=(RgTransferPass const &other) = delete;
        RgTransferPass &operator=(RgTransferPass &&other) noexcept;

        ~RgTransferPass();

        std::unordered_set<ResourceIdType> getInputResources() const override;
        std::unordered_set<ResourceIdType> getOutputResources() const override;

        inline BufferWrite const &getWriteOperation() const;
    };
}

#include "RgTransferPass.inl"