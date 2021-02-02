#include <stdexcept>

namespace garlic::clove::serialiser {
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int>>
    Node &Node::operator=(T scalar) {
        value = static_cast<float>(scalar);
        return *this;
    }

    template<typename T, std::enable_if_t<!std::is_arithmetic_v<T>, int>>
    Node &Node::operator=(const T &object) {
        std::string const prevName{ name };
        *this = serialise(object);
        name  = prevName;

        return *this;
    }

    template<typename T>
    T Node::as() {
        if constexpr(std::is_arithmetic_v<T>) {
            if(float *floatValue{ std::get_if<float>(&value) }) {
                return *floatValue;
            }else{
                throw std::runtime_error{ "Node is not holding a value!" };
            }
        } else {
            static_assert(false, "Unsupported type conversion");
        }
    }
}