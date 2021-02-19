#include <sstream>
#include <stdexcept>

namespace garlic::clove::serialiser {
    template<typename T>
    Node &Node::operator=(T const &value) {
        if constexpr(std::is_arithmetic_v<T> || isKeyType<T>) {
            type = Type::Scalar;
            nodes.resize(1);
            nodes[0].setValue(value);
        } else {
            *this = serialise(value);
        }

        return *this;
    }

    template<typename T>
    void Node::pushBack(T const &scalar) {
        if(type != Type::Sequence) {
            //TODO: Handle lossless conversion
            nodes.clear();
            type = Type::Sequence;
        }

        Node node{};
        node = scalar;
        nodes.push_back(std::move(node));
    }

    Node::Type Node::getType() const {
        return type;
    }

    std::string Node::getKey() const {
        return scalar;
    }

    Node::VectorType const &Node::getChildren() const {
        return nodes;
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
            return std::stof(nodes[0].scalar);
        } else if constexpr(std::is_same_v<std::string, T>) {
            if(type != Type::Scalar) {
                throw std::runtime_error{ "Node is not holding a value!" };
            }
            return nodes[0].scalar;
        } else {
            if(type != Type::Sequence && type != Type::Map) {
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
            std::stringstream stream;
            stream.precision(std::numeric_limits<T>::max_digits10);
            stream << value;
            scalar = stream.str();
        } else if constexpr(isKeyType<T>) {
            scalar = value;
        } else {
            throw std::runtime_error{ "setValue called without a scalar value." };
        }
    }
}