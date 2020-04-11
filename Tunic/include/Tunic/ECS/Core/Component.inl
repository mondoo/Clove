namespace tnc::ecs {
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
			pointer->base = nullptr;
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

	template<typename DerivedClassType>
	void Component<DerivedClassType>::attachPointer(ComponentPtr<DerivedClassType>* ptr) {
		pointers.push_back(ptr);
	}

	template<typename DerivedClassType>
	void Component<DerivedClassType>::detachPointer(ComponentPtr<DerivedClassType>* ptr) {
		pointers.erase(std::find(pointers.begin(), pointers.end(), ptr));
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr() = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(ComponentType* component) {
		base = component;
		this->component = component;

		if(base != nullptr) {
			base->attachPointer(this);
		}
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(const ComponentPtr& other) {
		copy(std::move(other));
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(ComponentPtr&& other) noexcept {
		move(std::move(other));
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>& ComponentPtr<ComponentType>::operator=(const ComponentPtr& other) {
		copy(std::move(other));
		return *this;
	}

	template<typename ComponentType>
	ComponentPtr<ComponentType>& ComponentPtr<ComponentType>::operator=(ComponentPtr&& other) noexcept {
		move(std::move(other));
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
			base->detachPointer(this);
			base = nullptr;
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
	void ComponentPtr<ComponentType>::copy(const ComponentPtr& other) {
		reset();

		base = other.base;
		component = other.component;

		if(base != nullptr) {
			base->attachPointer(this);
		}
	}

	template<typename ComponentType>
	void ComponentPtr<ComponentType>::move(ComponentPtr&& other) {
		reset();

		base = other.base;
		component = other.component;

		if(base != nullptr) {
			base->attachPointer(this);
		}
	}
}