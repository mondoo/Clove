#pragma once

namespace clv{
	namespace scene{
		class SceneNode;
		class RenderableSceneNode;
		class MeshSceneNode;
		class CameraSceneNode;
		class PointLightSceneNode;
		class DirectionalLightSceneNode;

		class Scene{
			//VARIABLES
		private:
			std::shared_ptr<SceneNode> rootNode;

			std::shared_ptr<CameraSceneNode> activeCamera;

			std::vector<std::shared_ptr<RenderableSceneNode>> renderables;

			std::vector<std::shared_ptr<PointLightSceneNode>> pointLights;
			std::vector<std::shared_ptr<DirectionalLightSceneNode>> directionalLights;

			//FUNCTIONS
		public:
			Scene();
			Scene(const Scene& other) = delete;
			Scene(Scene&& other) noexcept = delete;

			~Scene();

			void update(float deltaSeconds);

			CLV_API std::shared_ptr<MeshSceneNode> createMeshSceneNode();
			CLV_API std::shared_ptr<MeshSceneNode> createMeshSceneNode(std::shared_ptr<SceneNode> parent);

			CLV_API inline const std::vector<std::shared_ptr<RenderableSceneNode>>& getRenderables();

			CLV_API std::shared_ptr<CameraSceneNode> createCameraSceneNode();
			CLV_API std::shared_ptr<CameraSceneNode> createCameraSceneNode(std::shared_ptr<SceneNode> parent);

			CLV_API inline std::shared_ptr<CameraSceneNode> getActiveCamera();

			CLV_API std::shared_ptr<PointLightSceneNode> createPointLightSceneNode();
			CLV_API std::shared_ptr<PointLightSceneNode> createPointLightSceneNode(std::shared_ptr<SceneNode> parent);

			CLV_API inline const std::vector<std::shared_ptr<PointLightSceneNode>>& getPointLights();

			CLV_API std::shared_ptr<DirectionalLightSceneNode> createDirectionalLightSceneNode();
			CLV_API std::shared_ptr<DirectionalLightSceneNode> createDirectionalLightSceneNode(std::shared_ptr<SceneNode> parent);

			CLV_API inline const std::vector<std::shared_ptr<DirectionalLightSceneNode>>& getDirectionalLights();

			Scene& operator=(const Scene& other) = delete;
			Scene& operator=(Scene&& other) noexcept = delete;

		private:
			template<typename T>
			std::shared_ptr<T> createNode(std::shared_ptr<SceneNode> parent);
		};
	}
}

#include "Scene.inl"