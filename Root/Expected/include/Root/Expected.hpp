#include <cassert>

#pragma once

namespace garlic::inline root {
    /**
     * @brief Allows Expected to be constructed as an error properly 
     * if Expected's T and E are the same type.
     * @tparam E The error type
     */
    template<typename E>
    class Unexpected {
        //VARIABLES
    public:
        E value;

        //FUNCTIONS
    public:
        Unexpected() noexcept = delete;
        Unexpected(E const &value) noexcept
            : value{ value } {
        }
        Unexpected(E &&value) noexcept
            : value(std::move(value)) {
        }

        Unexpected(Unexpected const &other) noexcept = default;
        Unexpected(Unexpected &&other) noexcept      = default;

        Unexpected &operator=(Unexpected const &other) noexcept = default;
        Unexpected &operator=(Unexpected &&other) noexcept = default;

        ~Unexpected() noexcept = default;
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
            : value{ value } {
        }
        Expected(T &&value) noexcept
            : value{ std::move(value) } {
        }
        Expected(Unexpected<E> const &error) noexcept
            : error{ error.value }
            , ok{ false } {
        }
        Expected(Unexpected<E> &&error) noexcept
            : error{ std::move(error.value) }
            , ok{ false } {
        }

        Expected(Expected const &other) noexcept = default;
        Expected(Expected &&other) noexcept      = default;

        Expected &operator=(Expected const &other) noexcept = default;
        Expected &operator=(Expected &&other) noexcept = default;

        ~Expected() noexcept {}

        T &getValue() & {
            if(!ok) {
                throw error;
            }
            return value;
        }
        T const &getValue() const & {
            if(!ok) {
                throw error;
            }
            return value;
        }
        T &&getValue() && {
            if(!ok) {
                throw error;
            }
            return std::move(value);
        }
        T const &&getValue() const && {
            if(!ok) {
                throw error;
            }
            return std::move(value);
        }

        E &getError() & {
            assert(!ok);
            return error;
        }
        E const &getError() const & {
            assert(!ok);
            return error;
        }
        E &&getError() && {
            assert(!ok);
            return std::move(error);
        }
        E const &&getError() const && {
            assert(!ok);
            return std::move(error);
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