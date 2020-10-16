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
        Expected(T const &value) noexcept
            : value(value) {
        }
        Expected(T &&value) noexcept
            : value(std::move(value)) {
        }
        Expected(Unexpected<E> const &error) noexcept
            : error(error.value)
            , ok(false) {
        }
        Expected(Unexpected<E> &&error) noexcept
            : error(std::move(error.value))
            , ok(false) {
        }

        Expected(Expected const &other) noexcept = default;
        Expected(Expected &&other) noexcept      = default;

        Expected &operator=(Expected const &other) noexcept = default;
        Expected &operator=(Expected &&other) noexcept = default;

        ~Expected() noexcept {}

        T const &getValue() const & {
            return value;
        }

        E const &getError() const & {
            return error;
        }

        bool hasValue() const noexcept {
            return ok;
        }
        operator bool() const noexcept {
            return hasValue();
        }
    };
}

#include "Expected.inl"