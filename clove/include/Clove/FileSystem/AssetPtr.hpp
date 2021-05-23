#pragma once

#include <filesystem>
#include <optional>

namespace garlic::clove {
    /**
     * @brief Points to an asset on disk. Can load the asset when required.
     * @tparam AssetType 
     */
    template<typename AssetType>
    class AssetPtr {
        //VARIABLES
    private:
        std::filesystem::path assetPath{};

        std::optional<AssetType> asset{};

        //FUNCTIONS
    public:
        AssetPtr();

        AssetPtr(AssetPtr const &other) = delete;
        AssetPtr(AssetPtr &&other) noexcept;

        AssetPtr &operator=(AssetPtr const &other) = delete;
        AssetPtr &operator=(AssetPtr &&other) noexcept;

        ~AssetPtr();

        /**
         * @brief Returns true if this has a valid path to an asset.
         * @return 
         */
        bool isValid() const;

        /**
         * @brief Returns true if this has loaded the asset and there for contains a valid object.
         * @return 
         */
        bool isLoaded() const;
    };
}

#include "AssetPtr.inl"