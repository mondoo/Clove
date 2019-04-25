#pragma once

namespace clv{
	namespace loader{
		struct MeshInfo{
			std::vector<float> vertexData;
			std::vector<unsigned int> indices;

			std::vector<math::Vector3f> verticies;
			std::vector<math::Vector2f> texCoords;
			std::vector<math::Vector3f> normals;
		};

		class MeshLoader{
			//VARIABLES
		private:
			static MeshInfo emptyMeshInfo;
			static std::unordered_map<std::string, MeshInfo> loadedMeshes;

			//FUNCTIONS
		public:
			static const MeshInfo& loadOBJ(const std::string& filepath);

		};
	}
}