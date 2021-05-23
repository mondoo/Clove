#include <Clove/FileSystem/AssetPtr.hpp>
#include <Clove/Rendering/Renderables/Mesh.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

TEST(AssetPtrTest, ProperlyDefaultInitialises) {
    AssetPtr<Mesh> meshPtr{};

    EXPECT_FALSE(meshPtr.isValid());
    EXPECT_FALSE(meshPtr.isLoaded());
}

TEST(AssetPtrTest, CanInitialiseWithAFilePath) {
    AssetPtr<Mesh> meshPtr{ ASSET_DIR "/cube.obj" };

    EXPECT_TRUE(meshPtr.isValid());
    EXPECT_FALSE(meshPtr.isLoaded());
}