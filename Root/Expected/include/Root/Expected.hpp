#pragma once

namespace garlic::inline root {
    /**
     * @brief Allows Expected to be constructed as an error properly 
     * if Expected's T and E are the same type.
     * @tparam T 
     */
    template<typename E>
    class Unexpected {
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

        //FUNCTIONS
    public:
        Expected() = default;
        Expected(T value)
            : value(std::move(value)) {
        }
        Expected(Unexpected<E> error)
            : error(error.value) {
        }
        ~Expected() = default;

        T getValue() {
            return value;
        }

        E getError() {
            return error;
        }
    };
}

#include "Expected.inl"