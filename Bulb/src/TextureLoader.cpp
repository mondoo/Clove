#include "Bulb/TextureLoader.hpp"

#include <stb_image.h>

namespace blb::TextureLoader {
    LoadedTextureData loadTexture(std::string_view texturePath) {
        LoadedTextureData data{};
        void* localBuffer = stbi_load(texturePath.data(), &data.dimensions.x, &data.dimensions.y, &data.channels, STBI_rgb_alpha);
        data.channels     = STBI_rgb_alpha; //Make sure the channels are what we forced it to be
        data.buffer       = std::unique_ptr<void, std::function<void(void*)>>(localBuffer, [](void* buffer) {
            stbi_image_free(buffer);
        });
        return data;
    }
}