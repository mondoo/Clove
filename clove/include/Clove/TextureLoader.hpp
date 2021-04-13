#pragma once

#include <Clove/Expected.hpp>
#include <Clove/Maths/Vector.hpp>
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

namespace garlic::clove::TextureLoader {
    struct LoadedTextureData {
        std::unique_ptr<void, std::function<void(void *)>> buffer; /**< Point to the raw pixel data of the texture. */
        vec2ui dimensions{ 0, 0 }; /**< Size of the loaded texture. */
        int32_t channels{ 0 }; /**< How many chanels the texture has (i.e. RGBA is 4). */
    };
}

namespace garlic::clove::TextureLoader {
    /**
     * @brief Loads a texture. Returning the result or the reason for failure.
     * @param path Path to texture.
     * @return 
     */
    Expected<LoadedTextureData, std::runtime_error> loadTexture(std::filesystem::path const &path);
}