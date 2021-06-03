#pragma once

#include <filesystem>
#include <unordered_map>

namespace garlic::clove {
    /**
     * @brief Manages a file system that will not change even on different
     * platforms / install locations or even map into a BLOB or archive.
     */
    class VirtualFileSystem {
        //VARIABLES
    private:
        std::unordered_map<std::string, std::filesystem::path> mountedPaths{};

        //FUNCTIONS
    public:
        VirtualFileSystem();

        VirtualFileSystem(VirtualFileSystem const &other) = delete;
        VirtualFileSystem(VirtualFileSystem &&other) noexcept;

        VirtualFileSystem &operator=(VirtualFileSystem const &other) = delete;
        VirtualFileSystem &operator=(VirtualFileSystem &&other) noexcept;

        ~VirtualFileSystem();

        /**
         * @brief Mounts a system path to an alias.
         * @details Note that system paths are mounted as: std::filesystem::path::make_prefered
         * @param systemPath System path to mount.
         * @param alias The alias to give the full system path.
         */
        void mount(std::filesystem::path systemPath, std::string const &alias);

        /**
         * @brief Resolves a vfs path into a system path.
         * @details For example mounting 'data' as 'C:/files/data' then resolving
         * '/data/model.obj' will return 'C:/files/data/model.obj'.
         * @param vfsPath A path inside the VFS. THe path should begin with a mounted alias.
         * @return The resolved system path.
         */
        std::filesystem::path resolve(std::filesystem::path const &vfsPath);
    };
}