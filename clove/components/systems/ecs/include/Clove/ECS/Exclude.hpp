#pragma once

namespace garlic::clove {
    template<typename... Types>
    struct Exclude {
        static size_t constexpr size{ sizeof...(Types) };
    };
}