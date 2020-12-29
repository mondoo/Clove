#pragma once

#include "Clove/ECS/ComponentContainer.hpp"

#include <tuple>

namespace garlic::clove {
    template<typename... Types>
    struct Exclude {
        static size_t constexpr size{ sizeof...(Types) };
    };
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

        template<typename CallableType>
        void forEach(CallableType &&callable) {
            for(auto entity : *drivingContainer) {
                if constexpr(sizeof...(ExcludedTypes) > 0) {
                    if((std::get<ComponentContainer<ExcludedTypes> *>(excludedContainers)->hasComponent(entity) || ...)) {
                        continue;
                    }
                }

                if((std::get<ComponentContainer<ComponentTypes> *>(containerViews)->hasComponent(entity) && ...)) {
                    callable(std::get<ComponentContainer<ComponentTypes> *>(containerViews)->getComponent(entity)...);
                }
            }
        }

        //Begin / end?
        //[] operator?

    private:
        ComponentContainerInterface *chooseDrivingContainer() {
            return std::min({ std::get<ComponentContainer<ComponentTypes> *>(containerViews)... }, [](auto const *const lhs, auto const *const rhs) {
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

        template<typename CallableType>
        void forEach(CallableType &&callable) {
            for(auto entity : *container) {
                //TODO: Just iterate components here.
                callable(container->getComponent(entity));
            }
        }
    };

    //Deduction guidline for non excluding views.
    template<typename... Ts>
    ComponentView(Ts...) -> ComponentView<Exclude<>, Ts...>;
}

#include "ComponentView.inl"