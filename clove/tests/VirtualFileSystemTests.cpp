#include <Clove/FileSystem/VirtualFileSystem.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

TEST(VirtualFileSystemTest, CanMountAndResolveAFilePath) {
    std::filesystem::path const mountPath{ "C:/Users/Test/Some/Full/Path" };
    std::filesystem::path const emptyPath{};

    VirtualFileSystem vfs{};

    vfs.mount(mountPath, "saves");

    EXPECT_EQ(mountPath, vfs.resolve("saves"));
    EXPECT_EQ(emptyPath, vfs.resolve("notMounted"));
}

TEST(VirtualFileSystemTest, CanResolveNestedPaths) {
    std::filesystem::path const mountPath{ "C:/Users/Test/Some/Full/Path" };

    VirtualFileSystem vfs{};

    vfs.mount(mountPath, "data");

    EXPECT_EQ(mountPath / "models", vfs.resolve("data/models"));
    EXPECT_EQ(mountPath / "models/cube.obj", vfs.resolve("data/models/cube.obj"));
}