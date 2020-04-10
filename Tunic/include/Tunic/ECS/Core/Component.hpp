#pragma once

#include "Tunic/ECS/Core/ECSTypes.hpp"

namespace tnc::ecs {
	template<typename DerivedClassType> class Component;
}

namespace tnc::ecs {
	template<typename ComponentType>
	class ComponentPtr {
		friend class Component<ComponentType>;

		//VARIABLES
	private:
		ComponentType* component = nullptr;

		//FUNCTIONS
	public:
		ComponentPtr();
		ComponentPtr(ComponentType* component);

		ComponentPtr(const ComponentPtr& other);
		ComponentPtr(ComponentPtr&& other) noexcept;

		ComponentPtr& operator=(const ComponentPtr& other);
		ComponentPtr& operator=(ComponentPtr&& other) noexcept;

		~ComponentPtr();

		bool isValid();

		void reset();

		ComponentType* get() const;

		ComponentType* operator->() const;
		ComponentType& operator*() const;

		operator bool() const;

	private:
		void attach(ComponentType* component);
	};

	class ComponentInterface {
		//FUNCTIONS
	public:
		virtual ~ComponentInterface() = default;

		virtual ComponentID getID() const = 0;
	};

	template<typename DerivedClassType>
	class Component : public ComponentInterface {
		friend class ComponentPtr<DerivedClassType>;

		//VARIABLES
	public:
		EntityID entityID = INVALID_ENTITY_ID; //TODO: Make private

	private:
		std::list<ComponentPtr<DerivedClassType>*> pointers;

		//FUNCTIONS
	public:
		Component();

		Component(const Component& other);
		Component(Component&& other) noexcept;

		Component& operator=(const Component& other);
		Component& operator=(Component&& other) noexcept;

		virtual ~Component();

		static ComponentID id();

		virtual ComponentID getID() const override;
	};
}

#include "Component.inl"