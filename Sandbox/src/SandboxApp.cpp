#include <Clove.h>

//Clove
#include "Clove/Mesh.h"
#include "Clove/Object.h"
#include "Clove/Camera.h"

#include "Clove/Rendering/Renderer.h"

#include "Clove/Window.h"

#include "Clove/Events/MouseEvent.h"

//Vendor
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	clv::Object cube;
	clv::Object lightCube;
	clv::Camera cam;

	float r = 0.0f;
	float rot = 0.0f;
	float increment = 0.01f;

	float yaw = 0.0f;
	float pitch = 0.0f;

	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;

	bool firstMouse = true;

	//FUNCTIONS
public:
	ExampleLayer() 
		: Layer("Sanbox render test"){
	}

	virtual void onAttach() override{
		cube = clv::Object(clv::Mesh("res/Objects/cube.obj"));
		cube.setPosition(glm::vec3(0.0f, 0.0f, -500.0f));
		cube.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

		lightCube = clv::Object(clv::Mesh("res/Objects/cube.obj"));
		lightCube.setPosition(glm::vec3(200.0f, 200.0f, -900.0f));
		lightCube.setScale(glm::vec3(25.0f, 25.0f, 25.0f));
	}

	virtual void onDetach() override{
		
	}

	virtual void onUpdate() override{
		const float camSpeed = 10.0f;

		cam.update(pitch, yaw);

		glm::vec3 cameraPosition = cam.getPosition();

		const glm::vec3 front = cam.getFront();
		if(clv::Input::isKeyPressed(clv::Key::W)){
			cameraPosition += camSpeed * front;
		} else if(clv::Input::isKeyPressed(clv::Key::S)){
			cameraPosition -= camSpeed * front;
		}

		const glm::vec3 up = cam.getUp();
		if(clv::Input::isKeyPressed(clv::Key::Space)){
			cameraPosition += camSpeed * up;
		} else if(clv::Input::isKeyPressed(clv::Key::C)){
			cameraPosition -= camSpeed * up;
		}

		const glm::vec3 right = cam.getRight();
		if(clv::Input::isKeyPressed(clv::Key::A)){
			cameraPosition -= camSpeed * right;
		} else if(clv::Input::isKeyPressed(clv::Key::D)){
			cameraPosition += camSpeed * right;
		}

		cam.setPosition(cameraPosition);

		glm::mat4 view = cam.getLookAt();
		glm::mat4 proj = glm::perspective(45.0f, 16.0f / 9.0f, 1.0f, -1.0f);

		cube.setRotation(glm::vec3(0.0f, 1.0f, 0.0f), rot);

		cube.objectMesh->setAmbientStrength(0.1f);
		cube.objectMesh->setLightPosition(lightCube.getPosition());
		cube.objectMesh->setViewPosition(cameraPosition);
		cube.draw(clv::Application::get().getWindow().getRenderer(), view, proj);
		lightCube.draw(clv::Application::get().getWindow().getRenderer(), view, proj);

		if(r > 1.0f || r < 0.0f){
			increment = -increment;
		}
		r += increment;
		rot += 0.5f;
	
		if(clv::Input::isKeyPressed(clv::Key::Escape)){
			clv::Application::get().stop();
		}
	}

	virtual void onImGuiRender() override{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	virtual void onEvent(clv::Event& e) override{
		clv::EventDispatcher dispatcher(e);
		dispatcher.dispatch<clv::MouseMovedEvent>(CLV_BIND_FUNCTION_1P(&ExampleLayer::onMouseMoved, this));
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