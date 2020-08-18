#include "Bulb/TextureLoader.hpp"

#include <stb_image.h>

namespace blb::TextureLoader {
    LoadedTextureData loadTexture(std::string_view texturePath) {
        stbi_set_flip_vertically_on_load(1); //Note: Might need to change this when other APIs come into play

        int width;
        int height;
        int channels;
        void* localBuffer = stbi_load(texturePath.data(), &width, &height, &channels, STBI_rgb_alpha);

        LoadedTextureData data{};
        data.buffer     = std::unique_ptr<void, std::function<void(void*)>>(localBuffer, [](void* buffer) {
            stbi_image_free(buffer);
        });
        data.dimensions = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        data.channels   = STBI_rgb_alpha;//Make sure the channels are what we forced it to be

        return data;
    }
}