#include "clvpch.hpp"
#include "Scene.hpp"

#include "Clove/Scene/SceneNode.hpp"

namespace clv::scene{
	Scene::Scene(){
		rootNode = std::make_shared<SceneNode>();

		//Define the global scale factor
		//rootNode->setScale(math::Vector3f(100.0f));
	}

	Scene::~Scene() = default;

	void Scene::addNode(const std::shared_ptr<SceneNode>& node){
		rootNode->addChild(node);
	}

	void Scene::update(float deltaSeconds){
		rootNode->update(deltaSeconds);
	}
}