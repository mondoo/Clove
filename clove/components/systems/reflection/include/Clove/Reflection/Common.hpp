#pragma once

#include <any>
#include <string>
#include <vector>

namespace garlic::clove {
    struct Field {
        std::string name{};
        size_t size{ 0 };
        size_t offset{ 0 };

        std::any value{};
    };

    struct Type {
        std::string name{};
        size_t size{ 0 };

        std::vector<Field> fields{};
    };
}

namespace garlic::clove {
    template<typename T>
    struct TypeInfo {};

    template<typename T>
    Type reflect(T const &object) {
        return {};
    }
}