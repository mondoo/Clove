#include <type_traits>

namespace clove::reflection {
    namespace internal {
        template<typename T, typename Tuple>
        struct HasType;
        template<typename T, typename... Us>
        struct HasType<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};

        template<typename Type, size_t index, typename MemberTupleType, typename FunctorType>
        void constexpr forEachMember(MemberTupleType const &members, FunctorType const &functor) {
            if constexpr(index < TypeInfo<Type>::memberCount) {
                functor(std::get<index>(members));
                forEachMember<Type, index + 1>(members, functor);
            }
        }
    }

    template<typename AttributeType, typename ClassType>
    bool constexpr hasAttribute(TypeInfo<ClassType> const &typeInfo) {
        return internal::HasType<AttributeType, std::decay_t<decltype(typeInfo.attributes)>>::value;
    }

    template<typename AttributeType, typename MemberType>
    bool constexpr hasAttribute(MemberType const &member) {
        return internal::HasType<AttributeType, std::decay_t<decltype(member.attributes)>>::value;
    }

    template<typename Type, typename FunctorType>
    void constexpr forEachMember(FunctorType &&functor) {
        internal::forEachMember<Type, 0>(TypeInfo<Type>::getMembers(), std::forward<FunctorType>(functor));
    }
}