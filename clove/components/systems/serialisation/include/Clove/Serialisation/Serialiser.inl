namespace garlic::clove{
    template<typename T>
    void Serialiser::push(std::string_view name, T const &type) {
        Node node{ serialise(type) };
        node.name = name;

        if(head == nullptr) {
            head  = &root;
            *head = std::move(node);
        } else {
            if(auto *children{ std::get_if<std::vector<Node>>(&head->value) }) {
                children->emplace_back(std::move(node));
            } else {
                head->value = std::vector<Node>{ std::move(node) };
            }
        }
    }
}