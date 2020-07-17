namespace clv {
    template<typename DestType, typename SourceType>
    DestType* polyCast(SourceType* source) {
#if CLV_DEBUG
        auto* result = dynamic_cast<DestType*>(source);
        CLV_ASSERT(result != nullptr, "Cast failed");
        return result;
#else
        return static_cast<DestType*>(source);
#endif
    }
}