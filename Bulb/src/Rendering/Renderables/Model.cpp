#include "Bulb/Rendering/Renderables/Model.hpp"

#include "Bulb/Rendering/Renderables/Mesh.hpp"

namespace blb::rnd{
	Model::Model(std::vector<std::shared_ptr<Mesh>> meshes)
		: meshes(std::move(meshes)){
	}

	Model::Model(const Model& other){
		meshes.clear();
		for(const std::shared_ptr<Mesh>& mesh : other.meshes) {
			meshes.emplace_back(std::make_shared<Mesh>(*mesh));
		}
	}

	Model::Model(Model&& other) = default;

	Model& Model::operator=(const Model& other){
		meshes.clear();
		for(const std::shared_ptr<Mesh>& mesh : other.meshes) {
			meshes.emplace_back(std::make_shared<Mesh>(*mesh));
		}

		return *this;
	}

	Model& Model::operator=(Model&& other) = default;

	Model::~Model() = default;
	
	const std::vector<std::shared_ptr<Mesh>>& Model::getMeshes() const {
		return meshes;
	}

	std::shared_ptr<Mesh>& Model::operator[](size_t index) {
		return meshes[index];
	}
}