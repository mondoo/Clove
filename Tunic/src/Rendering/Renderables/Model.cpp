#include "Tunic/Rendering/Renderables/Model.hpp"

#include "Tunic/Rendering/Renderables/Mesh.hpp"

namespace tnc::rnd{
	Model::Model(std::vector<std::shared_ptr<Mesh>> meshes)
		: meshes(std::move(meshes)){
	}

	Model::Model(const Model& other) = default;

	Model::Model(Model&& other) = default;

	Model& Model::operator=(const Model& other) = default;

	Model& Model::operator=(Model&& other) = default;

	Model::~Model() = default;
	
	const std::vector<std::shared_ptr<Mesh>>& Model::getMeshes() const {
		return meshes;
	}

	std::shared_ptr<Mesh>& Model::operator[](size_t index) {
		return meshes[index];
	}
}