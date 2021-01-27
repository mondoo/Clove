#pragma once

#include <Clove/Expected.hpp>
#include <Clove/Maths/Vector.hpp>
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

namespace garlic::clove::TextureLoader {
    struct LoadedTextureData {
        std::unique_ptr<void, std::function<void(void *)>> buffer;
        vec2ui dimensions{ 0, 0 };
        int32_t channels{ 0 };
    };
}

namespace garlic::clove::TextureLoader {
    Expected<LoadedTextureData, std::runtime_error> loadTexture(std::filesystem::path const &path);
}