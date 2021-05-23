#include <Clove/FileSystem/AssetPtr.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

class MockFile {
public:
    bool isLoaded{ false };
};
TEST(AssetPtrTest, ProperlyDefaultInitialises) {
    AssetPtr<MockFile> asset{};

    EXPECT_FALSE(asset.isValid());
    EXPECT_FALSE(asset.isLoaded());
}

TEST(AssetPtrTest, CanInitialiseWithAFilePath) {
    AssetPtr<MockFile> asset{ "random/file/path.txt" };

    EXPECT_TRUE(asset.isValid());
    EXPECT_FALSE(asset.isLoaded());
}

    EXPECT_TRUE(meshPtr.isValid());
    EXPECT_FALSE(meshPtr.isLoaded());
}