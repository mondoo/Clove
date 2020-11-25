namespace garlic::clove {
    template<typename T>
    void Keyboard::trimBuffer(std::queue<T> &buffer) {
        while(buffer.size() > bufferSize) {
            buffer.pop();
        }
    }
}