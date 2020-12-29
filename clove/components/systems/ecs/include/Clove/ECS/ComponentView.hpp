#pragma once

#include "Clove/ECS/ComponentContainer.hpp"

#include <tuple>

namespace garlic::clove {
    template<typename... ComponentTypes>
    class ComponentView {
        //VARIABLES
    private:
        std::tuple<ComponentContainer<ComponentTypes> *...> containerViews{};
        ComponentContainerInterface *drivingContainer{ nullptr }; /**< Used to index into other views. Ideally the container with the least entities */

        //FUNCTIONS
    public:
        ComponentView() = delete;
        ComponentView(std::tuple<ComponentContainer<ComponentTypes> *...> containerViews)
            : containerViews{ std::move(containerViews) } {
            drivingContainer = std::min({ std::get<ComponentContainer<ComponentTypes> *>(containerViews)... }, [](auto const *const lhs, auto const *const rhs) {
                return lhs->size() < rhs->size();
            });
        }
        //TODO: Ctors

        template<typename CallableType>
        void forEach(CallableType &&callable /* , Exclude<ExcludeTypes...> */) {
            for(auto entity : *drivingContainer) {
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