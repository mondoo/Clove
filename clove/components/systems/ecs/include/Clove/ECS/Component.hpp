#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/Entity.hpp"

#include <list>

namespace garlic::clove {
    template<typename ComponentType>
    class ComponentPtr;

    template<typename ComponentType>
    class ComponentContainer;
}

namespace garlic::clove {
    /**
	 * @brief The base class for all Components.
	 * @tparam DetivedClassType The type of class that derives from this class.
	 * @see	TransformComponent
	 */
    template<typename DerivedClassType>
    class Component {
        friend class ComponentPtr<DerivedClassType>;
        friend class ComponentContainer<DerivedClassType>;

        //VARIABLES
    private:
        Entity entity{ NullEntity };

        //FUNCTIONS
    public:
        Component();

        Component(Component const &other);
        Component(Component &&other) noexcept;

        Component &operator=(Component const &other);
        Component &operator=(Component &&other) noexcept;

        virtual ~Component();

        static ComponentID id();
        ComponentID getComponentID() const;

        Entity getEntity() const;
    };
}

#include "Component.inl"