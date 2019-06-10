#include <Clove.hpp>

//Clove
#include "Clove/Scene/Camera.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Scene/Scene.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/ECS/Entity.hpp"
#include "Clove/Scene/Lights/PointLight.hpp"
#include "Clove/Input/Input.hpp"

#include "Clove/ECS/Entity.hpp"
#include "Clove/ECS/Components/TransformComponent.hpp"
#include "Clove/ECS/Components/RenderableComponent.hpp"

class TestEntity : public clv::ecs::Entity{
public:
	TestEntity(clv::ecs::EntityID ID)
		: clv::ecs::Entity(ID){

		std::unique_ptr<clv::gfx::Mesh> mesh = std::make_unique<clv::gfx::Mesh>();
		mesh->setDiffuseTexture("res/Textures/container2.png");
		mesh->setSpecularTexture("res/Textures/container2_specular.png");
		mesh->entityID = ID;

		std::unique_ptr<clv::ecs::TransformComponent> trans = std::make_unique<clv::ecs::TransformComponent>();
		trans->entityID = ID;

		components[clv::ecs::TransformComponent::ID] = std::move(trans);
		components[clv::gfx::Mesh::ID] = std::move(mesh);
	}

	void setPosition(const clv::math::Vector3f& pos){
		getComponent<clv::ecs::TransformComponent>()->setLocalPosition(pos);
	}

	void setRotation(const std::pair<clv::math::Vector3f, float>& rotation){
		getComponent<clv::ecs::TransformComponent>()->setLocalRotation(rotation);
	}

	void addChild(Entity* entity){
		getComponent<clv::ecs::TransformComponent>()->addChild(entity->getComponent<clv::ecs::TransformComponent>());
	}
};

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	float rotDelta = 0.0f;

	std::shared_ptr<clv::scene::Camera> cam;
	//std::array<std::shared_ptr<TestEntity>, 1 /*100*/> entities;
	std::shared_ptr<clv::scene::PointLight> light;
	//std::shared_ptr<clv::ecs::Entity> lightEntity;

	TestEntity* ent1 = nullptr;
	TestEntity* ent2 = nullptr;
	TestEntity* ent3 = nullptr;

	bool firstMouse = false;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;

	//FUNCTIONS
public:
	ExampleLayer()
		: Layer("Sanbox render test"){
	}

	virtual void onAttach() override{
		cam = std::make_shared<clv::scene::Camera>();
		std::shared_ptr<clv::scene::Scene> scene = clv::Application::get().getScene();
		
		clv::ecs::EntityID entID1 = clv::Application::get().getManager().createEntity<TestEntity>();
		clv::ecs::EntityID entID2 = clv::Application::get().getManager().createEntity<TestEntity>();
		clv::ecs::EntityID entID3 = clv::Application::get().getManager().createEntity<TestEntity>();
		
		ent1 = dynamic_cast<TestEntity*>(clv::Application::get().getManager().getEntity(entID1));
		ent2 = dynamic_cast<TestEntity*>(clv::Application::get().getManager().getEntity(entID2));
		ent3 = dynamic_cast<TestEntity*>(clv::Application::get().getManager().getEntity(entID3));


		//ent1->addChild(*ent2);

		ent1->setPosition({ 0.0f, 0.0f, 0.0f });
		ent2->setPosition({ 0.0f, 0.0f, 3.0f });
		ent3->setPosition({ 0.0f, 3.0f, 0.0f });

		ent1->addChild(ent2);
		ent2->addChild(ent3);

		//clv::Application::get().getManager().destroyEntity(entID3);

		light = std::make_unique<clv::scene::PointLight>();
		//lightEntity = std::make_unique<clv::ecs::Entity>();

		scene->addNode(cam);
		scene->addNode(light);

		light->setPosition({ 0.0f, 2.0f, -6.0f });
		//light->addChild(lightEntity);
		//lightEntity->mesh->setDiffuseTexture("res/Textures/container2.png");
		//lightEntity->setScale({ 0.25f, 0.25f, 0.25f });
	}

	virtual void onDetach() override{

	}

	virtual void onUpdate() override{
		const float camSpeed = 0.1f;
		clv::math::Vector3f cameraPosition = cam->getPosition();

		//cam->updateFront(pitch, yaw); //TODO: proper first person implementation

		const clv::math::Vector3f front = cam->getFront();
		if(clv::input::isKeyPressed(clv::Key::W)){
			cameraPosition += camSpeed * front;
		} else if(clv::input::isKeyPressed(clv::Key::S)){
			cameraPosition -= camSpeed * front;
		}

		const clv::math::Vector3f up = cam->getUp();
		if(clv::input::isKeyPressed(clv::Key::Space)){
			cameraPosition += camSpeed * up;
		} else if(clv::input::isKeyPressed(clv::Key::C)){
			cameraPosition -= camSpeed * up;
		}

		const clv::math::Vector3f right = cam->getRight();
		if(clv::input::isKeyPressed(clv::Key::A)){
			cameraPosition -= camSpeed * right;
		} else if(clv::input::isKeyPressed(clv::Key::D)){
			cameraPosition += camSpeed * right;
		}

		if(clv::input::isKeyPressed(clv::Key::Q)){
			yaw -= camSpeed * 10.0f;
		} else if(clv::input::isKeyPressed(clv::Key::E)){
			yaw += camSpeed * 10.0f;
		}

		cam->setPosition(cameraPosition);
		cam->updateFront(0.0f, yaw);

		ent1->setRotation({ { 0.0f, 1.0f, 0.0f }, rotDelta });

		/*for(auto& entity : entities){
			entity->setRotation();
		}*/
		rotDelta += 0.01f;

		if(clv::input::isKeyPressed(clv::Key::Escape)){
			clv::Application::get().stop();
		}
	}

	virtual void onImGuiRender() override{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	virtual void onEvent(clv::Event& e) override{
		//clv::EventDispatcher dispatcher(e);
		//dispatcher.dispatch<clv::MouseMovedEvent>(CLV_BIND_FUNCTION_1P(&ExampleLayer::onMouseMoved, this));
	}

	bool onMouseMoved(clv::MouseMovedEvent e){
		if(firstMouse){
			lastMouseX = e.getX();
			lastMouseY = e.getY();
			firstMouse = false;
		}

		float xoffset = e.getX() - lastMouseX;
		float yoffset = lastMouseY - e.getY();
		lastMouseX = e.getX();
		lastMouseY = e.getY();

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if(pitch > 89.0f){
			pitch = 89.0f;
		}
		if(pitch < -89.0f){
			pitch = -89.0f;
		}

		return true;
	}
};

class SandBox : public clv::Application{
public:
	SandBox(){
		pushLayer(std::make_shared<ExampleLayer>(ExampleLayer()));
	}
	~SandBox(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new SandBox;
}