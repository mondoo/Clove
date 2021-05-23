#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    template<typename AssetType>
    AssetPtr<AssetType>::AssetPtr() = default;

    template<typename AssetType>
    AssetPtr<AssetType>::AssetPtr(std::filesystem::path assetPath, std::function<AssetType(std::filesystem::path const &)> loadFunction)
        : assetPath{ std::move(assetPath) }
        , loadFunction{ std::move(loadFunction) } {
    }

    template<typename AssetType>
    AssetPtr<AssetType>::AssetPtr(AssetPtr &&other) noexcept = default;

    template<typename AssetType>
    AssetPtr<AssetType> &AssetPtr<AssetType>::operator=(AssetPtr &&other) noexcept = default;

    template<typename AssetType>
    AssetPtr<AssetType>::~AssetPtr() = default;

    template<typename AssetType>
    bool AssetPtr<AssetType>::isValid() const {
        return !assetPath.empty();
    }

    template<typename AssetType>
    bool AssetPtr<AssetType>::isLoaded() const {
        return asset.has_value();
    }

    template<typename AssetType>
    AssetType const &AssetPtr<AssetType>::get() {
        CLOVE_ASSERT(isValid(), "{0}: AssetPtr requires a valid path before it can load");

        if(!isLoaded()) {
            asset = loadFunction(assetPath);
        }

        return asset.value();
    }
}