#pragma once

namespace garlic::inline root {
    /**
     * @brief Allows Expected to be constructed as an error properly 
     * if Expected's T and E are the same type.
     * @tparam T 
     */
    template<typename E>
    class Unexpected {
    public:
        E value;
    };

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
        union {
            T value;
            E error;
        };
        bool ok{ true };

        //FUNCTIONS
    public:
        Expected() noexcept = default;
        Expected(T value) noexcept
            : value(std::move(value)) {
        }
        Expected(Unexpected<E> error) noexcept
            : error(error.value) {
        }

        Expected(Expected const &other) noexcept = default;
        Expected(Expected &&other) noexcept      = default;

        Expected &operator=(Expected const &other) noexcept = default;
        Expected &operator=(Expected &&other) noexcept = default;

        ~Expected() noexcept {}

        T getValue() {
            return value;
        }

        E getError() {
            return error;
        }
    };
}

#include "Expected.inl"