namespace clv{
	namespace scene{
		inline std::shared_ptr<CameraSceneNode> Scene::getActiveCamera(){
			return activeCamera;
		}

		inline const std::vector<std::shared_ptr<PointLightSceneNode>>& Scene::getPointLights(){
			return pointLights;
		}

		inline const std::vector<std::shared_ptr<DirectionalLightSceneNode>>& Scene::getDirectionalLights(){
			return directionalLights;
		}

		template<typename T>
		inline std::shared_ptr<T> Scene::createNode(std::shared_ptr<SceneNode> parent){
			std::shared_ptr<T> outNode = std::make_shared<T>();

			CLV_ASSERT(parent, "Invalid parent in {0} adding mesh to root node", __FUNCTION__);

			if(parent){
				parent->addChild(outNode);
			} else{
				rootNode->addChild(outNode);
			}

			return outNode;
		}
	}
}