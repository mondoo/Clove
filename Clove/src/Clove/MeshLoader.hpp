#pragma once

namespace clv{
	struct MeshInfo{
		std::vector<math::Vector3f> verticies;
		std::vector<math::Vector2f> texCoords;
		std::vector<math::Vector3f> normals;

		std::vector<unsigned int> indices;
	};

	bool loadOBJ(const std::string& filepath, MeshInfo& outVertexInfo);
}