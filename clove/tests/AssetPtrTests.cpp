#include <Clove/FileSystem/AssetPtr.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

class MockFile {
public:
    bool isLoaded{ false };
};

namespace {
    MockFile loadMockFile(std::filesystem::path const &filePath) {
        return MockFile{ true };
    }
}

TEST(AssetPtrTest, ProperlyDefaultInitialises) {
    AssetPtr<MockFile> asset{};

    EXPECT_FALSE(asset.isValid());
    EXPECT_FALSE(asset.isLoaded());
}

TEST(AssetPtrTest, CanInitialiseWithAFilePath) {
    AssetPtr<MockFile> asset{ "random/file/path.txt", &loadMockFile };

    EXPECT_TRUE(asset.isValid());
    EXPECT_FALSE(asset.isLoaded());
}

TEST(AssetPtrTest, CanLoadFileWhenRequested) {
    AssetPtr<MockFile> emptyPtr{};

    EXPECT_DEATH(MockFile const &invalidFile{ emptyPtr.get() }, "");

    AssetPtr<MockFile> asset{ "random/file/path.txt", &loadMockFile };
    AssetPtr<MockFile> const constAsset{ "random/file/path.txt", &loadMockFile };

    ASSERT_TRUE(asset.isValid());
    ASSERT_FALSE(asset.isLoaded());

    ASSERT_TRUE(constAsset.isValid());
    ASSERT_FALSE(constAsset.isLoaded());

    MockFile const &loadedFile{ asset.get() };
    MockFile &fileRef{ *asset };

    EXPECT_TRUE(asset.isLoaded());
    EXPECT_TRUE(asset->isLoaded);
    EXPECT_TRUE(loadedFile.isLoaded);
    EXPECT_TRUE(fileRef.isLoaded);

    MockFile const &loadedConstFile{ constAsset.get() };

    EXPECT_TRUE(constAsset.isLoaded());
    EXPECT_TRUE(loadedConstFile.isLoaded);
}

TEST(AssetPtrTest, CanPointToSameAssetButOnlyLoadOnce) {
    int32_t callCount{ 0 };

    auto const loadMockFileCount = [&callCount](std::filesystem::path const &path) mutable {
        ++callCount;
        return MockFile{ true };
    };

    AssetPtr<MockFile> asset{ "random/file/path.txt", loadMockFileCount };
    AssetPtr<MockFile> assetCopy{ asset };

    ASSERT_TRUE(asset.isValid());
    EXPECT_EQ(asset.isValid(), assetCopy.isValid());

    MockFile const &loadedFile{ assetCopy.get() };

    ASSERT_TRUE(assetCopy.isLoaded());
    EXPECT_EQ(asset.isLoaded(), assetCopy.isLoaded());
    EXPECT_TRUE(asset->isLoaded);
    EXPECT_EQ(callCount, 1);

    AssetPtr<MockFile> invalidAsset{};
    AssetPtr<MockFile> invalidCopy{ invalidAsset };

    EXPECT_FALSE(invalidAsset.isValid());
    EXPECT_FALSE(invalidCopy.isValid());
}