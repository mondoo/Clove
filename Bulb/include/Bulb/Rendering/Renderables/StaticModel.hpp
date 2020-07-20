#pragma once

namespace blb::rnd {
	class Mesh;
}

namespace blb::rnd {
	class StaticModel {
		//VARIABLES
	private:
		std::vector<std::shared_ptr<Mesh>> meshes;

		//FUNCTIONS
	public:
		StaticModel() = delete;
		StaticModel(std::vector<std::shared_ptr<Mesh>> meshes);
		
		StaticModel(const StaticModel& other);
		StaticModel(StaticModel&& other);

		StaticModel& operator=(const StaticModel& other);
		StaticModel& operator=(StaticModel&& other);

		~StaticModel();

		const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;

		std::shared_ptr<Mesh>& operator[](size_t index);
	};
}