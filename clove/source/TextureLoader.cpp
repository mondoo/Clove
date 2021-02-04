#include "Clove/TextureLoader.hpp"

#include <stb_image.h>

namespace garlic::clove::TextureLoader {
    Expected<LoadedTextureData, std::runtime_error> loadTexture(std::filesystem::path const &path) {
        int width{ 0 };
        int height{ 0 };
        int channels{ 0 };
        void *localBuffer{ stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha) };

        if(localBuffer == nullptr){
            return Unexpected{ std::runtime_error{ "Failed to load texture!" } };
        }

        return LoadedTextureData{
            .buffer = std::unique_ptr<void, std::function<void(void *)>>(localBuffer, [](void *buffer) { stbi_image_free(buffer); }),
            .dimensions = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) },
            .channels   = STBI_rgb_alpha,//Make sure the channels are what we forced it to be
        };
    }
}