#pragma once

namespace blb::rnd {
	class Mesh;
}

namespace blb::rnd {
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

		std::shared_ptr<Mesh>& operator[](size_t index);
	};
}