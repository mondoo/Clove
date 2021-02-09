#include <stdexcept>

namespace garlic::clove::serialiser {
    template<typename T>
    Node &Node::operator=(T const &value) {
        if constexpr(std::is_arithmetic_v<T> || isKeyType<T>()) {
            type = Type::Scalar;
            nodes.resize(1);
            nodes[0].setValue(value);
        } else {
            *this = serialise(value);
        }

        return *this;
    }

    Node::Type Node::getType() const {
        return type;
    }

    std::string Node::getKey() const {
        return scalar;
    }

    Node::VectorType::iterator Node::begin() {
        return nodes.begin();
    }

    Node::VectorType::iterator Node::end() {
        return nodes.end();
    }

    Node::VectorType::const_iterator Node::begin() const {
        return nodes.begin();
    }

    Node::VectorType::const_iterator Node::end() const {
        return nodes.end();
    }

    template<typename T>
    T Node::as() const {
        if constexpr(std::is_arithmetic_v<T>) {
            if(type != Type::Scalar) {
                throw std::runtime_error{ "Node is not holding a value!" };
            }
            std::stof(nodes[0].scalar);
        } else if constexpr(std::is_same_v<std::string, T>) {
            if(type != Type::Scalar) {
                throw std::runtime_error{ "Node is not holding a value!" };
            }
            return nodes[0].scalar;
        } else {
            if(type != Type::Sequence || type != Type::Map) {
                throw std::runtime_error{ "Node is not holding any nodes!" };
            }
            return deserialise<T>(*this);
        }
    }

    template<typename T>
    void Node::setValue(T const &value) {
        type = Type::None;
        nodes.clear();

        if constexpr(std::is_arithmetic_v<T>) {
            scalar = std::to_string(value);
        } else if constexpr(isKeyType<T>()) {
            scalar = value;
        } else {
            throw std::runtime_error{ "setValue called without a scalar value." };
        }
    }

    template<typename T>
    bool constexpr Node::isKeyType() {
        return std::is_same_v<std::string, T> || std::is_same_v<std::string_view, T> || std::is_same_v<char *, std::decay<T>>;
    }
}