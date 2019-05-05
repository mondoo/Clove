#include "clvpch.hpp"
#include "Scene.hpp"

#include "Clove/Scene/SceneNode.hpp"
#include "Clove/Scene/MeshSceneNode.hpp"
#include "Clove/Scene/CameraSceneNode.hpp"
#include "Clove/Scene/Lights/PointLightSceneNode.hpp"
#include "Clove/Scene/Lights/DirectionalLightSceneNode.hpp"

namespace clv{
	namespace scene{
		Scene::Scene(){
			rootNode = std::make_shared<SceneNode>();

			//Define the global scale factor
			rootNode->setScale(math::Vector3f(100.0f));
		}

		Scene::~Scene() = default;

		void Scene::update(float deltaSeconds){
			rootNode->update(deltaSeconds);
		}

		std::shared_ptr<MeshSceneNode> Scene::createMeshSceneNode(){
			return createMeshSceneNode(rootNode);
		}

		std::shared_ptr<MeshSceneNode> Scene::createMeshSceneNode(std::shared_ptr<SceneNode> parent){
			std::shared_ptr<MeshSceneNode> node = createNode<MeshSceneNode>(parent);
			renderables.push_back(node);
			return node;
		}

		std::shared_ptr<CameraSceneNode> Scene::createCameraSceneNode(){
			return createCameraSceneNode(rootNode);
		}

		std::shared_ptr<CameraSceneNode> Scene::createCameraSceneNode(std::shared_ptr<SceneNode> parent){
			if(!activeCamera){
				activeCamera = createNode<CameraSceneNode>(parent);
			} else{
				CLV_WARN("Camera already created. Returning current camera");
			}

			return activeCamera;
		}

		std::shared_ptr<PointLightSceneNode> Scene::createPointLightSceneNode(){
			return createPointLightSceneNode(rootNode);
		}

		std::shared_ptr<PointLightSceneNode> Scene::createPointLightSceneNode(std::shared_ptr<SceneNode> parent){
			std::shared_ptr<PointLightSceneNode> light = createNode<PointLightSceneNode>(parent);
			pointLights.push_back(light);
			return light;
		}

		std::shared_ptr<DirectionalLightSceneNode> Scene::createDirectionalLightSceneNode(){
			return createDirectionalLightSceneNode(rootNode);
		}

		std::shared_ptr<DirectionalLightSceneNode> Scene::createDirectionalLightSceneNode(std::shared_ptr<SceneNode> parent){
			std::shared_ptr<DirectionalLightSceneNode> light = createNode<DirectionalLightSceneNode>(parent);
			directionalLights.push_back(light);
			return light;
		}
	}
}