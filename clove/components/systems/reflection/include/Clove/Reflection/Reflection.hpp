#pragma once

#include <Clove/Blackboard.hpp>
#include <optional>
#include <string>
#include <tuple>
#include <typeinfo>
#include <vector>

namespace clove::reflection {
    class AttributeContainer {
        //VARIABLES
    private:
        BlackBoard attributes{};

        //FUNCTIONS
    public:
        template<typename AttributeType>
        void add(AttributeType &&attribute) {
            attributes.setValue(typeid(AttributeType).hash_code(), std::forward<AttributeType>(attribute));
        }

        template<typename AttributeType>
        bool contains() const {
            return attributes.hasValue(typeid(AttributeType).hash_code());
        }

        template<typename AttributeType>
        std::optional<AttributeType> get() const {
            return attributes.getValue<AttributeType>(typeid(AttributeType).hash_code());
        }
    };

    using TypeId = decltype(std::declval<std::type_info>().hash_code());

    /**
     * @brief A member within a TypeInfo.
     */
    struct MemberInfo {
        std::string name{};
        TypeId id{};     /**< Type id for this member. If the member is reflected then this ID can be used to retrieve the TypeInfo struct. */
        size_t offset{}; /**< Offset of this member within the type it is contained in. */
        size_t size{};   /**< Total size of this member. */
        AttributeContainer attributes{};
    };

    /**
     * @brief Info about a reflected type.
     */
    struct TypeInfo {
        std::string name{};
        TypeId id{}; /**< Type id for this type info. Automatically generated using typeid */
        size_t size{};
        std::vector<MemberInfo> members;
        AttributeContainer attributes{};
    };

    namespace internal {
        class Registry {
            //VARIABLES
        private:
            std::vector<reflection::TypeInfo> types{};//TODO: Make into a map

            //FUNCTIONS
        public:
            //TODO: Ctors

            static Registry &get() {
                static Registry *instance{ nullptr };
                if(instance == nullptr) {
                    instance = new Registry{};
                }

                return *instance;
            }

            std::vector<reflection::TypeInfo> const &getRegisteredTypes() const {
                return types;
            }

            void addTypeInfo(reflection::TypeInfo typeInfo) {
                types.push_back(std::move(typeInfo));
            }
        };

        //TODO: inline
        template<size_t index, typename TupleType>
        static void populateAttributes(AttributeContainer &container, TupleType const &attributeTuple) {
            if constexpr(index < std::tuple_size_v<TupleType>) {
                container.add(std::get<index>(attributeTuple));

                populateAttributes<index + 1>(container, attributeTuple);
            }
        }


        template<typename ReflectType>
        struct CreatorHelper;
    }

    template<typename Type>
    TypeInfo const *getTypeInfo();

    /**
     * @brief Returns the TypeInfo with the provided typeName.
     * @param typeName 
     * @return 
     */
    TypeInfo const *getTypeInfo(std::string_view typeName);

    /**
     * @brief Returns the TypeInfo with the provided typeId.
     * @param typeId 
     * @return 
     */
    TypeInfo const *getTypeInfo(TypeId typeId);

    template<typename AttributeType>
    std::vector<TypeInfo const *> getTypesWithAttribute() {
        std::vector<TypeInfo const *> typeInfos{};

        for(auto &typeInfo : internal::Registry::get().getRegisteredTypes()) {
            if(typeInfo.attributes.contains<AttributeType>()) {
                typeInfos.push_back(&typeInfo);
            }
        }

        return typeInfos;
    }
}

#define INTERNAL_CLOVE_REFLECT_CAT2(a, b) a##b
#define INTERNAL_CLOVE_REFLECT_CAT(a, b) INTERNAL_CLOVE_REFLECT_CAT2(a, b)

#define CLOVE_REFLECT_BEGIN(classType, ...)                                                                                         \
    template<>                                                                                                                      \
    struct ::clove::reflection::internal::CreatorHelper<classType> {                                                                \
        CreatorHelper();                                                                                                            \
    };                                                                                                                              \
                                                                                                                                    \
    inline static ::clove::reflection::internal::CreatorHelper<classType> const INTERNAL_CLOVE_REFLECT_CAT(creator, __COUNTER__){}; \
                                                                                                                                    \
    ::clove::reflection::internal::CreatorHelper<classType>::CreatorHelper() {                                                      \
        using Type = classType;                                                                                                     \
        ::clove::reflection::TypeInfo info{};                                                                                       \
        info.name = #classType;                                                                                                     \
        info.id   = typeid(classType).hash_code();                                                                                  \
        info.size = sizeof(classType);                                                                                              \
        ::clove::reflection::internal::populateAttributes<0>(info.attributes, std::make_tuple(__VA_ARGS__));

#define CLOVE_REFLECT_MEMBER(member, ...)                                                                          \
    {                                                                                                              \
        ::clove::reflection::MemberInfo memberInfo{};                                                              \
        memberInfo.name   = #member;                                                                               \
        memberInfo.id     = typeid(Type::member).hash_code();                                                      \
        memberInfo.offset = offsetof(Type, member);                                                                \
        memberInfo.size   = sizeof(Type::member);                                                                  \
        ::clove::reflection::internal::populateAttributes<0>(memberInfo.attributes, std::make_tuple(__VA_ARGS__)); \
        info.members.push_back(std::move(memberInfo));                                                             \
    }

#define CLOVE_REFLECT_END                                                        \
    ::clove::reflection::internal::Registry::get().addTypeInfo(std::move(info)); \
    }                                                                            \
    ;

/**
 * @brief Allows reflection of private members within a class.
 */
#define CLOVE_REFLECT_PRIVATE(classType) \
    friend struct ::clove::reflection::internal::CreatorHelper<classType>;

#include "Reflection.inl"