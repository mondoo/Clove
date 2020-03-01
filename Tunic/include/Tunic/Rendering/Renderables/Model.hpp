#pragma once

namespace tnc::rnd {
	class Mesh;
}

namespace tnc::rnd {
	class Model {
		//VARIABLES
	private:
		std::vector<std::shared_ptr<Mesh>> meshes;

		//FUNCTIONS
	public:
		Model() = delete;
		Model(std::vector<std::shared_ptr<Mesh>> meshes);
		
		Model(const Model& other);
		Model(Model&& other);

		Model& operator=(const Model& other);
		Model& operator=(Model&& other);

		~Model();

		const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;
	};
}