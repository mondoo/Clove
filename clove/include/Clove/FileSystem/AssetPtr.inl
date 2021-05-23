namespace garlic::clove {
    template<typename AssetType>
    AssetPtr<AssetType>::AssetPtr() = default;

    template<typename AssetType>
    AssetPtr<AssetType>::AssetPtr(std::filesystem::path assetPath)
        : assetPath{ std::move(assetPath) } {
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
}