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

    struct MemberInfo {
        std::string name;
        size_t offset;
        size_t size;
    };

    struct TypeInfo {
        std::string name;
        std::vector<MemberInfo> members;
        AttributeContainer attributes{};
    };

    namespace internal {
        /**
         * @brief Provides information about a specific type.
         */
        template<typename T>
        struct TypeInfo;

        class Registry {
            //VARIABLES
        private:
            std::vector<reflection::TypeInfo> types{};

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
    }

    template<typename Type>
    TypeInfo getTypeInfo() {
        //TODO: Use registry to remain consistent
        return internal::TypeInfo<Type>::getTypeInfo();
    }

    TypeInfo const *getTypeInfo(std::string_view typeName);

    template<typename AttributeType>
    std::vector<TypeInfo const *> getTypesWithAttribute(){
        std::vector<TypeInfo const *> typeInfos{};
       
        for(auto &typeInfo : internal::Registry::get().getRegisteredTypes()) {
            if(typeInfo.attributes.contains<AttributeType>()) {
                typeInfos.push_back(&typeInfo);
            }
        }

        return typeInfos;
    }
}

#define CLOVE_REFLECT_BEGIN(classType, ...)                                  \
    template<>                                                               \
    struct clove::reflection::internal::TypeInfo<classType> {                \
        using Type = classType;                                              \
                                                                             \
        template<size_t>                                                     \
        struct Member;                                                       \
                                                                             \
        static inline auto const attributes{ std::make_tuple(__VA_ARGS__) }; \
        static std::string_view constexpr name{ #classType };                \
                                                                             \
        static size_t constexpr memberIndexOffset{ __COUNTER__ + 1 };

#define CLOVE_REFLECT_PROPERTY(property, ...)                                \
    template<>                                                               \
    struct Member<__COUNTER__ - memberIndexOffset> {                         \
        static std::string_view constexpr name{ #property };                 \
        static size_t constexpr offset{ offsetof(Type, property) };          \
        static size_t constexpr size{ sizeof(Type::property) };              \
                                                                             \
        static inline auto const attributes{ std::make_tuple(__VA_ARGS__) }; \
    };

#define CLOVE_REFLECT_END(classType)                                        \
    static size_t constexpr memberCount{ __COUNTER__ - memberIndexOffset }; \
                                                                            \
    TypeInfo() {                                                            \
        internal::Registry::get().addTypeInfo(getTypeInfo());               \
    }                                                                       \
                                                                            \
    static clove::reflection::TypeInfo getTypeInfo() {                      \
        clove::reflection::TypeInfo info{};                                 \
        info.name = name;                                                   \
        populateMemberArray<0>(info.members);                               \
        populateAttributes<0>(info.attributes);                             \
                                                                            \
        return info;                                                        \
    };                                                                      \
                                                                            \
private:                                                                    \
    template<size_t index>                                                  \
    static void populateMemberArray(std::vector<MemberInfo> &memberArray) { \
        if constexpr(index < memberCount) {                                 \
            MemberInfo info{};                                              \
            info.name   = Member<index>::name;                              \
            info.offset = Member<index>::offset;                            \
            info.size   = Member<index>::size;                              \
            memberArray.push_back(std::move(info));                         \
                                                                            \
            populateMemberArray<index + 1>(memberArray);                    \
        }                                                                   \
    }                                                                       \
                                                                            \
    template<size_t index>                                                  \
    static void populateAttributes(AttributeContainer &container) {         \
        if constexpr(index < std::tuple_size_v<decltype(attributes)>) {     \
            container.add(std::get<index>(attributes));                     \
                                                                            \
            populateAttributes<index + 1>(container);                       \
        }                                                                   \
    }                                                                       \
    }                                                                       \
    ;                                                                       \
    static clove::reflection::internal::TypeInfo<classType> const creator__##classType{};

/**
 * @brief Allows reflection of private members within a class.
 */
#define CLOVE_REFLECT_PRIVATE(classType) \
    friend struct clove::reflection::internal::TypeInfo<classType>;

#include "Reflection.inl"