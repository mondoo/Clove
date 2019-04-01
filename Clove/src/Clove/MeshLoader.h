#pragma once

namespace clv{
	struct MeshInfo{
		std::vector<glm::vec3> verticies;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;

		std::vector<unsigned int> indices;
	};

	bool loadOBJ(const std::string& filepath, MeshInfo& outVertexInfo);
}