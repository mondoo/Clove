namespace tnc::ecs {
	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr() = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(ComponentType* component) {
		attach(component);
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(const ComponentPtr& other) {
		reset();
		attach(other.component);
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(ComponentPtr&& other) noexcept {
		reset();
		attach(other.component);
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>& ComponentPtr<ComponentType>::operator=(const ComponentPtr& other) {
		reset();
		attach(other.component);

		return *this;
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>& ComponentPtr<ComponentType>::operator=(ComponentPtr&& other) noexcept {
		reset();
		attach(other.component);

		return *this;
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>::~ComponentPtr() {
		reset();
	}

	template<typename ComponentType>
	bool ComponentPtr<ComponentType>::isValid() const {
		return component != nullptr;
	}

	template<typename ComponentType>
	void ComponentPtr<ComponentType>::reset() {
		if(isValid()) {
			component->pointers.erase(std::find(component->pointers.begin(), component->pointers.end(), this));
			component = nullptr;
		}
	}

	template<typename ComponentType>
	ComponentType* ComponentPtr<ComponentType>::get() const {
		return component;
	}

	template<typename ComponentType>
	ComponentType* ComponentPtr<ComponentType>::operator->() const {
		return get();
	}

	template<typename ComponentType>
	ComponentType& ComponentPtr<ComponentType>::operator*() const {
		return *get();
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>::operator bool() const {
		return isValid();
	}

	template<typename ComponentType>
	void ComponentPtr<ComponentType>::attach(ComponentType* component) {
		component->pointers.push_back(this);
		this->component = component;
	}

	template<typename DerivedClassType>
	Component<DerivedClassType>::Component() = default;

	template<typename DerivedClassType>
	Component<DerivedClassType>::Component(const Component& other) = default;

	template<typename DerivedClassType>
	Component<DerivedClassType>::Component(Component&& other) noexcept {
		entityID = std::move(other.entityID);
		pointers = std::move(other.pointers);
	}

	template<typename DerivedClassType>
	Component<DerivedClassType>& Component<DerivedClassType>::operator=(const Component& other) = default;

	template<typename DerivedClassType>
	Component<DerivedClassType>& Component<DerivedClassType>::operator=(Component&& other) noexcept {
		entityID = std::move(other.entityID);
		pointers = std::move(other.pointers);

		return *this;
	}

	template<typename DerivedClassType>
	Component<DerivedClassType>::~Component() {
		for(auto* pointer : pointers) {
			pointer->component = nullptr;
		}
	}

	template<typename DerivedClassType>
	ComponentID Component<DerivedClassType>::id() {
		return typeid(DerivedClassType).hash_code();
	}

	template<typename DerivedClassType>
	ComponentID Component<DerivedClassType>::getID() const {
		return id();
	}
}