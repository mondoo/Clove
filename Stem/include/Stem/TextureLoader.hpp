#pragma once

namespace garlic::inline stem::TextureLoader {
    struct LoadedTextureData {
        std::unique_ptr<void, std::function<void(void *)>> buffer;
        clv::mth::vec2ui dimensions{ 0, 0 };
        int32_t channels{ 0 };
    };
}

namespace garlic::inline stem::TextureLoader {
    LoadedTextureData loadTexture(std::string_view texturePath);
}