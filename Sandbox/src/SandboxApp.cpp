#include <Clove.hpp>

//Clove
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/ECS/Entity.hpp"
#include "Clove/Input/Input.hpp"

#include "Clove/ECS/Entity.hpp"
#include "Clove/ECS/Components/TransformComponent.hpp"
#include "Clove/ECS/Components/MeshComponent.hpp"
#include "Clove/ECS/Components/LightComponent.hpp"
#include "Clove/ECS/Components/CameraComponent.hpp"

class TestEntity : public clv::ecs::Entity{
public:
	TestEntity(clv::ecs::EntityID ID)
		: clv::ecs::Entity(ID){

		clv::ecs::MeshComponent* mesh = addComponent<clv::ecs::MeshComponent>();
		mesh->setDiffuseTexture("res/Textures/container2.png");
		mesh->setSpecularTexture("res/Textures/container2_specular.png");

		clv::ecs::TransformComponent* trans = addComponent<clv::ecs::TransformComponent>();
	}

	void setPosition(const clv::math::Vector3f& pos){
		getComponent<clv::ecs::TransformComponent>()->setLocalPosition(pos);
	}

	void setRotation(const std::pair<clv::math::Vector3f, float>& rotation){
		getComponent<clv::ecs::TransformComponent>()->setLocalRotation(rotation);
	}

	void setScale(const clv::math::Vector3f& scale){
		getComponent<clv::ecs::TransformComponent>()->setLocalScale(scale);
	}

	void addChild(Entity* entity){
		getComponent<clv::ecs::TransformComponent>()->addChild(entity->getComponent<clv::ecs::TransformComponent>());
	}
};

class TestLight : public clv::ecs::Entity{
public:
	TestLight(clv::ecs::EntityID ID)
		: clv::ecs::Entity(ID){

		clv::ecs::LightComponent* light = addComponent<clv::ecs::LightComponent>();
		clv::ecs::TransformComponent* trans = addComponent<clv::ecs::TransformComponent>();
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

class TestCamera : public clv::ecs::Entity{
public:
	TestCamera(clv::ecs::EntityID ID)
		: clv::ecs::Entity(ID){

		clv::ecs::CameraComponent* cam = addComponent<clv::ecs::CameraComponent>();
		clv::ecs::TransformComponent* trans = addComponent<clv::ecs::TransformComponent>();
	}

	void setPosition(const clv::math::Vector3f& pos){
		getComponent<clv::ecs::TransformComponent>()->setLocalPosition(pos);
	}

	const clv::math::Vector3f& getPosition(){
		return getComponent<clv::ecs::TransformComponent>()->getLocalPosition();
	}

	const clv::math::Vector3f& getFront(){
		return getComponent<clv::ecs::CameraComponent>()->getFront();
	}

	const clv::math::Vector3f& getUp(){
		return getComponent<clv::ecs::CameraComponent>()->getUp();
	}

	const clv::math::Vector3f& getRight(){
		return getComponent<clv::ecs::CameraComponent>()->getRight();
	}

	void updateFront(float pitch, float yaw){
		getComponent<clv::ecs::CameraComponent>()->updateFront(pitch, yaw);
	}
};

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	float rotDelta = 0.0f;

	TestEntity* ent1 = nullptr;
	TestEntity* ent2 = nullptr;
	TestEntity* ent3 = nullptr;

	TestLight* lght1 = nullptr;
	TestEntity* ltEnt = nullptr;

	TestCamera* cam = nullptr;

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
		ent1 = clv::Application::get().getManager().createEntity<TestEntity>();
		ent2 = clv::Application::get().getManager().createEntity<TestEntity>();
		ent3 = clv::Application::get().getManager().createEntity<TestEntity>();

		lght1 = clv::Application::get().getManager().createEntity<TestLight>();
		ltEnt = clv::Application::get().getManager().createEntity<TestEntity>();

		cam = clv::Application::get().getManager().createEntity<TestCamera>();
	
		ent1->setPosition({ 0.0f, 0.0f, 0.0f });
		ent2->setPosition({ 0.0f, 0.0f, 3.0f });
		ent3->setPosition({ 0.0f, 3.0f, 0.0f });

		ent1->addChild(ent2);
		ent2->addChild(ent3);

		//clv::Application::get().getManager().destroyEntity(ent2->getID());

		lght1->addChild(ltEnt);
		ltEnt->setScale({ 0.25f, 0.25f, 0.25f });
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
		const float radius = 6.0f;
		lght1->setPosition({ cos(rotDelta) * radius, 2.0f, sin(rotDelta) * radius });

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