#pragma once

#include "Clove/ECS/ComponentContainer.hpp"

#include <tuple>

namespace garlic::clove {
    template<typename... Types>
    struct Exclude {
        static size_t constexpr size{ sizeof...(Types) };
    };

    template<typename FunctionType>
    struct FunctionTraits;

    template<typename RetType, typename... ParameterTypes>
    struct FunctionTraits<RetType(ParameterTypes...)> {
        using ReturnType               = RetType;
        using ParameterTypesTuple      = std::tuple<ParameterTypes...>;
        using DecayParameterTypesTuple = std::tuple<std::decay_t<ParameterTypes>...>;

        static inline size_t constexpr arity{ sizeof...(ParameterTypes) };
    };

    //Free function specialisation
    template<typename RetType, typename... ParameterTypes>
    struct FunctionTraits<RetType (*)(ParameterTypes...)> : public FunctionTraits<RetType(ParameterTypes...)> {};

    //Member function specialisation
    template<typename RetType, typename ClassType, typename... ParameterTypes>
    struct FunctionTraits<RetType (ClassType::*)(ParameterTypes...)> : public FunctionTraits<RetType(ParameterTypes...)> {};

    //Const member function specialisation
    template<typename RetType, typename ClassType, typename... ParameterTypes>
    struct FunctionTraits<RetType (ClassType::*)(ParameterTypes...) const> : public FunctionTraits<RetType(ParameterTypes...)> {};

    //Callable object specialisation
    template<typename CallableType>
    struct FunctionTraits : public FunctionTraits<decltype(&CallableType::operator())> {};
}

namespace garlic::clove {
    template<typename...>
    class ComponentView;

    //Multi-Component view specialisation.
    template<typename... ExcludedTypes, typename... ComponentTypes>
    class ComponentView<Exclude<ExcludedTypes...>, ComponentTypes...> {
        //VARIABLES
    private:
        std::tuple<ComponentContainer<ComponentTypes> *...> containerViews{};
        std::tuple<ComponentContainer<ExcludedTypes> *...> excludedContainers{};

        ComponentContainerInterface *drivingContainer{ nullptr }; /**< Used to index into other views. Ideally the container with the least entities */

        //FUNCTIONS
    public:
        ComponentView() = delete;

        ComponentView(std::tuple<ComponentContainer<ComponentTypes> *...> containerViews)
            : containerViews{ std::move(containerViews) }
            , drivingContainer{ chooseDrivingContainer() } {
        }

        ComponentView(std::tuple<ComponentContainer<ComponentTypes> *...> containerViews, std::tuple<ComponentContainer<ExcludedTypes> *...> excludedContainers)
            : containerViews{ std::move(containerViews) }
            , excludedContainers{ std::move(excludedContainers) }
            , drivingContainer{ chooseDrivingContainer() } {
        }
        //TODO: Ctors

        //Free functions / callables
        template<typename CallableType>
        void forEach(CallableType callable) {
            for(auto entity : *drivingContainer) {
                if constexpr(sizeof...(ExcludedTypes) > 0) {
                    if((std::get<ComponentContainer<ExcludedTypes> *>(excludedContainers)->hasComponent(entity) || ...)) {
                        continue;
                    }
                }

                if((std::get<ComponentContainer<ComponentTypes> *>(containerViews)->hasComponent(entity) && ...)) {
                    //Call with entity if it's the first argument
                    if constexpr(std::is_same_v<Entity, std::tuple_element_t<0, typename FunctionTraits<CallableType>::ParameterTypesTuple>>) {
                        callable(entity, std::get<ComponentContainer<ComponentTypes> *>(containerViews)->getComponent(entity)...);
                    } else {
                        callable(std::get<ComponentContainer<ComponentTypes> *>(containerViews)->getComponent(entity)...);
                    }
                }
            }
        }

        //Member functions
        template<typename FunctionType, typename ClassType>
        void forEach(FunctionType function, ClassType *object) {
            for(auto entity : *drivingContainer) {
                if constexpr(sizeof...(ExcludedTypes) > 0) {
                    if((std::get<ComponentContainer<ExcludedTypes> *>(excludedContainers)->hasComponent(entity) || ...)) {
                        continue;
                    }
                }

                if((std::get<ComponentContainer<ComponentTypes> *>(containerViews)->hasComponent(entity) && ...)) {
                    //Call with entity if it's the first argument
                    if constexpr(std::is_same_v<Entity, std::tuple_element_t<0, typename FunctionTraits<FunctionType>::ParameterTypesTuple>>) {
                        (object->*function)(entity, std::get<ComponentContainer<ComponentTypes> *>(containerViews)->getComponent(entity)...);
                    } else {
                        (object->*function)(std::get<ComponentContainer<ComponentTypes> *>(containerViews)->getComponent(entity)...);
                    }
                }
            }
        }

        //Begin / end?
        //[] operator?

    private:
        ComponentContainerInterface *chooseDrivingContainer() {
            return std::min(std::initializer_list<ComponentContainerInterface*>{ std::get<ComponentContainer<ComponentTypes> *>(containerViews)... }, [](auto const *const lhs, auto const *const rhs) {
                return lhs->size() < rhs->size();
            });
        }
    };

    //Single-Component view specialisation
    template<typename ComponentType>
    class ComponentView<Exclude<>, ComponentType> {
        //VARIABLES
    private:
        ComponentContainer<ComponentType> *container{};

        //FUNCTIONS
    public:
        ComponentView() = delete;

        ComponentView(ComponentContainer<ComponentType> *container)
            : container{ container } {
        }

        //TODO: Ctors

        //Free functions / callables
        template<typename CallableType>
        void forEach(CallableType callable) {
            for(auto entity : *container) {
                //TODO: Just iterate components here?

                //Call with entity if it's the first argument
                if constexpr(std::is_same_v<Entity, std::tuple_element_t<0, typename FunctionTraits<CallableType>::ParameterTypesTuple>>) {
                    callable(entity, container->getComponent(entity));
                } else {
                    callable(container->getComponent(entity));
                }
            }
        }

        //Member functions
        template<typename FunctionType, typename ClassType>
        void forEach(FunctionType function, ClassType *object) {
            for(auto entity : *container) {
                //TODO: Just iterate components here?

                //Call with entity if it's the first argument
                if constexpr(std::is_same_v<Entity, std::tuple_element_t<0, typename FunctionTraits<FunctionType>::ParameterTypesTuple>>) {
                    (object->*function)(entity, container->getComponent(entity));
                } else {
                    (object->*function)(container->getComponent(entity));
                }
            }
        }
    };

    //Deduction guidline for non excluding views.
    template<typename... Ts>
    ComponentView(Ts...) -> ComponentView<Exclude<>, Ts...>;
}

#include "ComponentView.inl"