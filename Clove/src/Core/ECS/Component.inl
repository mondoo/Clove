namespace clv::ecs{
	template<typename DerivedClassType>
	Component<DerivedClassType>::Component(const Component& other) = default;

	template<typename DerivedClassType>
	Component<DerivedClassType>::Component(Component&& other) noexcept = default;

	template<typename DerivedClassType>
	Component<DerivedClassType>& Component<DerivedClassType>::operator=(const Component& other) = default;

	template<typename DerivedClassType>
	Component<DerivedClassType>& Component<DerivedClassType>::operator=(Component&& other) noexcept = default;

	template<typename DerivedClassType>
	Component<DerivedClassType>::~Component() = default;

	template<typename DerivedClassType>
	ComponentID Component<DerivedClassType>::id(){
		return typeid(DerivedClassType).hash_code();
	}

	template<typename DerivedClassType>
	ComponentID Component<DerivedClassType>::getID() const{
		return id();
	}
}