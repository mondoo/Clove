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

    ASSERT_TRUE(asset.isValid());
    ASSERT_FALSE(asset.isLoaded());

    MockFile const &loadedFile{ asset.get() };

    EXPECT_TRUE(asset.isLoaded());
    EXPECT_TRUE(loadedFile.isLoaded);
}