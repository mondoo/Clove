namespace clove {
    template<typename T>
    T *ListAllocator::alloc() {
        return reinterpret_cast<T *>(alloc(sizeof(T), alignof(T)));
    }
}