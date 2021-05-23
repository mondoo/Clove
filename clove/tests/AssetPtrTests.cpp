#include <Clove/FileSystem/AssetPtr.hpp>
#include <Clove/Rendering/Renderables/Mesh.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

TEST(AssetPtrTest, ProperlyDefaultInitialises) {
    AssetPtr<Mesh> meshPtr{};

    EXPECT_FALSE(meshPtr.isValid());
    EXPECT_FALSE(meshPtr.isLoaded());
}

