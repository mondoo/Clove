#pragma once

#include <string>
#include <vector>

namespace garlic::clove {
    struct Member {
        std::string name{};
        size_t size{};
        size_t offset{};
    };

    struct Class {
        std::string name{};
        size_t size{};
        std::vector<Member> members;
    };
}

namespace garlic::clove {
    template<typename T>
    Class getClass(T const &object) {
        return {};
    }
}