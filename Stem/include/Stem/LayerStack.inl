namespace garlic::inline stem {
    std::vector<std::shared_ptr<Layer>>::iterator LayerStack::begin() {
        return layers.begin();
    }

    std::vector<std::shared_ptr<Layer>>::iterator LayerStack::end() {
        return layers.end();
    }

    std::vector<std::shared_ptr<Layer>>::reverse_iterator LayerStack::rbegin() {
        return layers.rbegin();
    }

    std::vector<std::shared_ptr<Layer>>::reverse_iterator LayerStack::rend() {
        return layers.rend();
    }
}