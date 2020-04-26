#include "Bulb/ECS/Components/InputComponent.hpp"

namespace blb::ecs {
	InputComponent::BindingId InputComponent::nextId = 0;

	InputComponent::InputComponent() = default;

	InputComponent::InputComponent(const InputComponent& other) = default;

	InputComponent::InputComponent(InputComponent&& other) noexcept {
		keyBindings			= std::move(other.keyBindings);
		mouseButtonBindings = std::move(other.mouseButtonBindings);
	}

	InputComponent& InputComponent::operator=(const InputComponent& other) = default;

	InputComponent& InputComponent::operator=(InputComponent&& other) noexcept {
		keyBindings			= std::move(other.keyBindings);
		mouseButtonBindings = std::move(other.mouseButtonBindings);

		return *this;
	}

	InputComponent::~InputComponent() = default;

	void InputComponent::unbind(BindingId id) {
		//Loop through key bindings
		for(auto&& idToIndexMap : keyIdToIndexMap) {
			if(auto idIter = idToIndexMap.second.find(id); idIter != idToIndexMap.second.end()) {
				std::vector<KeyBindingFunction>& keyVector = keyBindings[idToIndexMap.first];

				const size_t index		= idIter->second;
				const size_t lastIndex	= keyVector.size() - 1;

				if(index < lastIndex) {
					keyVector[index] = keyVector.back();
				}

				keyVector.pop_back();
				idToIndexMap.second.erase(id);

				//Update the index map so it knows about the moved component
				if(index < lastIndex) {
					auto movediter = std::find_if(idToIndexMap.second.begin(), idToIndexMap.second.end(), [lastIndex](const std::pair<BindingId, size_t>& keyValPair) {
						return (keyValPair.second == lastIndex);
					});
					const BindingId movedId = movediter->first;
					idToIndexMap.second[movedId] = index;
				}

				//Finished, return
				return;
			}
		}

		//Loop through mouse bindings
		for(auto&& idToIndexMap : buttonIdToIndexMap) {
			if(auto idIter = idToIndexMap.second.find(id); idIter != idToIndexMap.second.end()) {
				std::vector<KeyBindingFunction>& mouseVector = mouseButtonBindings[idToIndexMap.first];

				const size_t index		= idIter->second;
				const size_t lastIndex = mouseVector.size() - 1;

				if(index < lastIndex) {
					mouseVector[index] = mouseVector.back();
				}

				mouseVector.pop_back();
				idToIndexMap.second.erase(id);

				//Update the index map so it knows about the moved component
				if(index < lastIndex) {
					auto movediter = std::find_if(idToIndexMap.second.begin(), idToIndexMap.second.end(), [lastIndex](const std::pair<BindingId, size_t>& keyValPair) {
						return (keyValPair.second == lastIndex);
					});
					const BindingId movedId = movediter->first;
					idToIndexMap.second[movedId] = index;
				}

				//Finished, return
				return;
			}
		}
	}
}