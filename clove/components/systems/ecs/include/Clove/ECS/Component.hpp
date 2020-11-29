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
        Entity entity = NullEntity;

        std::list<ComponentPtr<DerivedClassType> *> pointers;

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

    private:
        void attachPointer(ComponentPtr<DerivedClassType> *ptr);
        void detachPointer(ComponentPtr<DerivedClassType> *ptr);
    };

    /**
	 * @brief Wraps a pointer to a Component.
	 * @details A ComponentPtr acts similar to a std::weak_ptr except that ComponentPtrs can't be locked.
	 * Once the Component this class points to has been removed from the World this
	 * class will act like a nullptr.
	 * @tparam ComponentType The type of Component to point to.
	 */
    template<typename ComponentType>
    class ComponentPtr {
        friend class Component<ComponentType>;

        //VARIABLES
    private:
        ComponentType *component{ nullptr };

        //FUNCTIONS
    public:
        ComponentPtr();
        ComponentPtr(ComponentType *component);

        ComponentPtr(ComponentPtr const &other);
        ComponentPtr(ComponentPtr &&other) noexcept;

        ComponentPtr &operator=(ComponentPtr const &other);
        ComponentPtr &operator=(ComponentPtr &&other) noexcept;

        ~ComponentPtr();

        bool isValid() const;

        /**
		 * @brief Resets this ComponentPtr, turing it into a nullptr.
		 */
        void reset();

        ComponentType *get() const;

        ComponentType *operator->() const;
        ComponentType &operator*() const;

        operator bool() const;

    private:
        void copy(ComponentPtr const &other);
        void move(ComponentPtr &&other);
    };
}

#include "Component.inl"

//ComponentType == ComponentType
template<typename ComponentType>
inline bool operator==(garlic::clove::ComponentPtr<ComponentType> const &lptr, garlic::clove::ComponentPtr<ComponentType> const &rptr) {
    return lptr.get() == rptr.get();
}
template<typename ComponentType>
inline bool operator==(garlic::clove::ComponentPtr<ComponentType> const &ptr, ComponentType const *object) {
    return ptr.get() == object;
}
template<typename ComponentType>
inline bool operator==(garlic::clove::ComponentPtr<ComponentType> const &ptr, ComponentType const &object) {
    return *(ptr.get()) == object;
}

//ComponentType != ComponentType
template<typename ComponentType>
inline bool operator!=(garlic::clove::ComponentPtr<ComponentType> const &lptr, garlic::clove::ComponentPtr<ComponentType> const &rptr) {
    return !(lptr == rptr);
}
template<typename ComponentType>
inline bool operator!=(garlic::clove::ComponentPtr<ComponentType> const &ptr, ComponentType const *object) {
    return !(ptr == object);
}
template<typename ComponentType>
inline bool operator!=(garlic::clove::ComponentPtr<ComponentType> const &ptr, ComponentType const &object) {
    return !(ptr == object);
}

//ComponentType == nullptr_t
template<typename ComponentType>
inline bool operator==(garlic::clove::ComponentPtr<ComponentType> const &ptr, std::nullptr_t object) {
    return ptr.get() == object;
}

//ComponentType != nullptr_t
template<typename ComponentType>
inline bool operator!=(garlic::clove::ComponentPtr<ComponentType> const &ptr, std::nullptr_t object) {
    return !(ptr == object);
}
