#pragma once

#include "Clove/ECS/ComponentContainer.hpp"

#include <tuple>

namespace garlic::clove {
    template<typename... ComponentTypes>
    class ComponentView {
        //VARIABLES
    public: //TEMP
    //private:
        std::tuple<ComponentContainer<ComponentTypes> *...> containerViews{};

        //FUNCTIONS
    public:
        ComponentView() = delete;
        ComponentView(std::tuple<ComponentContainer<ComponentTypes> *...> containerViews)
            : containerViews{ std::move(containerViews) } {
        }
        //TODO: Ctors

        template<typename CallableType>
        void forEach(CallableType callable/* , Exclude<ExcludeTypes...> */) {
            //TODO
        }

        //Begin / end?
        //[] operator?
    };
}

#include "ComponentView.inl"