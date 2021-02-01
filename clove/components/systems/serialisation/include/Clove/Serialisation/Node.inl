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
}