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

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	/*std::shared_ptr<clv::scene::CameraSceneNode> cam;

	std::shared_ptr<clv::scene::MeshSceneNode> cube;
	*/

	float rotDelta = 0.0f;

	std::shared_ptr<clv::scene::Camera> cam;
	std::shared_ptr<clv::ecs::Entity> entity;
	std::shared_ptr<clv::scene::PointLight> light;
	std::shared_ptr<clv::ecs::Entity> lightEntity;

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
		entity = std::make_shared<clv::ecs::Entity>();
		light = std::make_unique<clv::scene::PointLight>();
		lightEntity = std::make_unique<clv::ecs::Entity>();

		std::shared_ptr<clv::scene::Scene> scene = clv::Application::get().getScene();

		scene->addNode(entity);
		scene->addNode(cam);
		scene->addNode(light);

		entity->mesh->setDiffuseTexture("res/Textures/container2.png");
		entity->mesh->setSpecularTexture("res/Textures/container2_specular.png");
		entity->setPosition({ 0.0f, 0.0f, -4.0f });

		light->setPosition({ 0.0f, 2.0f, -6.0f });
		light->addChild(lightEntity);
		lightEntity->mesh->setDiffuseTexture("res/Textures/container2.png");
		lightEntity->setScale({ 0.25f, 0.25f, 0.25f });
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

		entity->setRotation({ { 0.0f, 1.0f, 0.0f }, rotDelta });
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