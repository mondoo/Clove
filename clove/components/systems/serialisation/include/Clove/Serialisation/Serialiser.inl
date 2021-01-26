namespace garlic::clove {
    template<typename T>
    void Serialiser::push(std::string_view name, T const &object) {
        Node node{ serialise(object) };
        node.name  = name;
        push(std::move(node));
    }
}