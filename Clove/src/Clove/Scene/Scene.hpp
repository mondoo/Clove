#pragma once

namespace clv::scene{
	class SceneNode;
	class Camera;

	class Scene{
		//VARIABLES
	private:
		std::shared_ptr<SceneNode> rootNode;

		//FUNCTIONS
	public:
		Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) noexcept = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) noexcept = delete;
		~Scene();

		void addNode(const std::shared_ptr<SceneNode>& node); //TODO: Add or create?

		void update(float deltaSeconds);
	};
}