#pragma once

namespace garlic::inline root {
    /**
     * @brief Wraps either a value of type T or an error of type E.
     * @details Expected is used to return from functions either an
     * expected value or an error if the function fails.
     * @tparam T The type of the expected value.
     * @tparam E The type of the error. For example, can be an error
     * code or an exception.
     */
    template<typename T, typename E>
    class Expected {
        //VARIABLES
    private:
        T value;

        //FUNCTIONS
    public:
        Expected() = default;
        Expected(T value)
            : value(std::move(value)) {
        }

        T getValue() {
            return value;
        }
    };
}

#include "Expected.inl"