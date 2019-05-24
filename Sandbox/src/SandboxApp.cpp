#include <Clove.hpp>

//Clove
#include "Clove/Scene/Camera.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Profiling/Timer.hpp"
#include "Clove/Scene/Scene.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Mesh.hpp"

#include "Clove/ECS/Entity.hpp"

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	/*std::shared_ptr<clv::scene::CameraSceneNode> cam;

	std::shared_ptr<clv::scene::MeshSceneNode> cube;
	*/

	float rotDelta = 0.0f;

	std::shared_ptr<clv::scene::Camera> cam;
	std::shared_ptr<clv::ecs::Entity> entity;

	//FUNCTIONS
public:
	ExampleLayer()
		: Layer("Sanbox render test"){
	}

	virtual void onAttach() override{
		entity = std::make_shared<clv::ecs::Entity>();
		cam = std::make_shared<clv::scene::Camera>();

		std::shared_ptr<clv::scene::Scene> scene = clv::Application::get().getScene();

		scene->addNode(entity);
		scene->addNode(cam);

		entity->mesh->setTexture("res/Textures/container2.png");
		entity->setPosition({ 0.0f, 0.0f, -4.0f });
	}

	virtual void onDetach() override{

	}

	virtual void onUpdate() override{
		const float camSpeed = 0.1f;
		clv::math::Vector3f cameraPosition = cam->getPosition();

		const clv::math::Vector3f front = cam->getFront();
		if(clv::input::isKeyPressed(clv::Key::W)){
			cameraPosition -= camSpeed * front;
		} else if(clv::input::isKeyPressed(clv::Key::S)){
			cameraPosition += camSpeed * front;
		}

		const clv::math::Vector3f up = cam->getUp();
		if(clv::input::isKeyPressed(clv::Key::Space)){
			cameraPosition -= camSpeed * up;
		} else if(clv::input::isKeyPressed(clv::Key::C)){
			cameraPosition += camSpeed * up;
		}

		const clv::math::Vector3f right = cam->getRight();
		if(clv::input::isKeyPressed(clv::Key::A)){
			cameraPosition += camSpeed * right;
		} else if(clv::input::isKeyPressed(clv::Key::D)){
			cameraPosition -= camSpeed * right;
		}

		cam->setPosition(cameraPosition);

		entity->setRotation({ { 0.0f, 1.0f, 0.0f }, rotDelta });
		rotDelta += 0.01f;
	}

	virtual void onImGuiRender() override{

	}

	virtual void onEvent(clv::Event& e) override{

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