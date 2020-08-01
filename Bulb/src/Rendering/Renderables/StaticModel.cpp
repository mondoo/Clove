#include "Bulb/Rendering/Renderables/StaticModel.hpp"

#include "Bulb/Rendering/Renderables/Mesh.hpp"

namespace blb::rnd{
	StaticModel::StaticModel(std::vector<std::shared_ptr<Mesh>> meshes)
		: meshes(std::move(meshes)){
	}

	StaticModel::StaticModel(const StaticModel& other){
		meshes.clear();
		for(const std::shared_ptr<Mesh>& mesh : other.meshes) {
			meshes.emplace_back(std::make_shared<Mesh>(*mesh));
		}
	}

	StaticModel::StaticModel(StaticModel&& other) = default;

	StaticModel& StaticModel::operator=(const StaticModel& other){
		meshes.clear();
		for(const std::shared_ptr<Mesh>& mesh : other.meshes) {
			meshes.emplace_back(std::make_shared<Mesh>(*mesh));
		}

		return *this;
	}

	StaticModel& StaticModel::operator=(StaticModel&& other) = default;

	StaticModel::~StaticModel() = default;
	
	const std::vector<std::shared_ptr<Mesh>>& StaticModel::getMeshes() const {
		return meshes;
	}

	std::shared_ptr<Mesh>& StaticModel::operator[](size_t index) {
		return meshes[index];
	}
}