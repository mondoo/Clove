#include "Clove/FileSystem/VirtualFileSystem.hpp"

#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VirtualFileSystem::VirtualFileSystem() = default;

    VirtualFileSystem::VirtualFileSystem(VirtualFileSystem &&other) noexcept = default;

    VirtualFileSystem &VirtualFileSystem::operator=(VirtualFileSystem &&other) noexcept = default;

    VirtualFileSystem::~VirtualFileSystem() = default;

    void VirtualFileSystem::mount(std::filesystem::path systemPath, std::string const &alias) {
        mountedPaths[alias] = systemPath;
    }

    std::filesystem::path VirtualFileSystem::resolve(std::string const &alias) {
        if(mountedPaths.contains(alias)){
            return mountedPaths.at(alias);
        }else{
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Unknown alias: {1}. Returning empty path", CLOVE_FUNCTION_NAME, alias);
            return {};
        }
    }
}