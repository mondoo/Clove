#include <Clove/FileSystem/VirtualFileSystem.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

TEST(VirtualFileSystemTest, CanMountAndResolveAFilePath) {
    std::filesystem::path mountPath{ "C:/Users/Test/Some/Full/Path" };
    std::filesystem::path emptyPath{};

    VirtualFileSystem vfs{};

    vfs.mount(mountPath, "saves");

    EXPECT_EQ(mountPath, vfs.resolve("saves"));
    EXPECT_EQ(emptyPath, vfs.resolve("notMounted"));
}