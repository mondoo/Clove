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
    T Node::as() const {
        if constexpr(std::is_arithmetic_v<T>) {
            if(float const *floatValue{ std::get_if<float>(&value) }) {
                return *floatValue;
            } else {
                throw std::runtime_error{ "Node is not holding a value!" };
            }
        } else {
            if(std::vector<Node> const *nodeValue{ std::get_if<std::vector<Node>>(&value) }){
                return deserialise<T>(*this);
            }else{
                throw std::runtime_error{ "Node is not holding any nodes!" };
            }
        }
    }
}