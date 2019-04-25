#pragma once

#include "Clove/Scene/RenderableSceneNode.hpp"

namespace clv{
	namespace scene{
		class MeshSceneNode : public RenderableSceneNode{
			//VARIABLES
		private:
			std::string meshPath;

			//FUNCTIONS
		public:
			CLV_API MeshSceneNode();
			CLV_API MeshSceneNode(const MeshSceneNode& other);
			CLV_API MeshSceneNode(MeshSceneNode&& other) noexcept;
			
			CLV_API MeshSceneNode(const std::string& meshPath);
			CLV_API MeshSceneNode(const std::string& meshPath, std::shared_ptr<Material> material);

			CLV_API ~MeshSceneNode();

			CLV_API void setMesh(const std::string& meshPath);

			CLV_API MeshSceneNode& operator=(const MeshSceneNode& other);
			CLV_API MeshSceneNode& operator=(MeshSceneNode&& other) noexcept;

		private:
			void createModelData(const std::string& meshPath);
		};
	}
}