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

        void mount(std::filesystem::path systemPath, std::string const &alias);

        std::filesystem::path resolve(std::string const &alias);
    };
}