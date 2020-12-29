#pragma once

#include "Clove/ECS/ComponentContainer.hpp"

#include <tuple>

namespace garlic::clove {
    template<typename... ComponentTypes>
    class ComponentView {
        //VARIABLES
    private:
        std::tuple<ComponentContainer<ComponentTypes> *...> containerViews{};
        //TODO: Just using the first type at the moment but we should iterate through the shortest one
        std::tuple_element_t<0, std::tuple<ComponentContainer<ComponentTypes>...>> *drivingView{ nullptr };

        //FUNCTIONS
    public:
        ComponentView() = delete;
        ComponentView(std::tuple<ComponentContainer<ComponentTypes> *...> containerViews)
            : containerViews{ std::move(containerViews) }
            , drivingView{ std::get<0>(containerViews) } {
        }
        //TODO: Ctors

        template<typename CallableType>
        void forEach(CallableType &&callable /* , Exclude<ExcludeTypes...> */) {
            for(auto entity : *drivingView) {
                if constexpr(sizeof...(ComponentTypes) > 1) {
                    if((std::get<ComponentContainer<ComponentTypes> *>(containerViews)->hasComponent(entity) && ...)) {
                        callable(std::get<ComponentContainer<ComponentTypes> *>(containerViews)->getComponent(entity)...);
                    }
                } else {
                    //TODO: Just iterate components here.
                    callable(std::get<ComponentContainer<ComponentTypes> *>(containerViews)->getComponent(entity)...);
                }
            }
        }

        //Begin / end?
        //[] operator?
    };
}

#include "ComponentView.inl"