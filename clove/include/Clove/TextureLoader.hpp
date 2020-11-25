#pragma once

#include <Clove/Maths/Vector.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace garlic::clove::TextureLoader {
    struct LoadedTextureData {
        std::unique_ptr<void, std::function<void(void *)>> buffer;
        vec2ui dimensions{ 0, 0 };
        int32_t channels{ 0 };
    };
}

namespace garlic::clove::TextureLoader {
    LoadedTextureData loadTexture(std::string_view texturePath);
}