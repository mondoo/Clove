#include <Clove.h>

//Clove
#include "Clove/Object.h"
#include "Clove/Mesh.h"
#include "Clove/Rendering/API/Material.h"

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
	std::shared_ptr<clv::Material> cubeMaterial;

	clv::Object sphere;
	std::shared_ptr<clv::Material> sphereMaterial;

	clv::Object monkey;
	std::shared_ptr<clv::Material> monkeyMaterial;

	clv::Object lightCube;
	std::shared_ptr<clv::Material> lightMaterial;

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
		cubeMaterial = std::make_shared<clv::Material>(clv::Material("res/Textures/container2.png"));
		cube = clv::Object(clv::Mesh("res/Objects/cube.obj", cubeMaterial));
		cube.setPosition(glm::vec3(0.0f, 0.0f, -300));
		cube.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

		sphereMaterial = std::make_shared<clv::Material>(clv::Material());
		sphere = clv::Object(clv::Mesh("res/Objects/sphere.obj", sphereMaterial));
		sphere.setPosition(glm::vec3(-500, 0.0f, -900));
		sphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

		monkeyMaterial = std::make_shared<clv::Material>(clv::Material());
		monkey = clv::Object(clv::Mesh("res/Objects/monkey.obj", monkeyMaterial));
		monkey.setPosition(glm::vec3(500, 0.0f, -900));
		monkey.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

		lightMaterial = std::make_shared<clv::Material>(clv::Material());
		lightCube = clv::Object(clv::Mesh("res/Objects/cube.obj", lightMaterial));
		lightCube.setPosition(glm::vec3(0, 200, -900));
		lightCube.setScale(glm::vec3(25.0f, 25.0f, 25.0f));

		//Material shiz
		//CUBE
		cubeMaterial->setSpecularTexture("res/Textures/container2_specular.png");

		cubeMaterial->setUniform1f("material.shininess", 32.0f);

		cubeMaterial->setUniform3f("light.ambient", glm::vec3(0.01f, 0.01f, 0.01f));
		cubeMaterial->setUniform3f("light.diffuse", glm::vec3(0.75f, 0.75f, 0.75f));
		cubeMaterial->setUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//SPHERE
		//sphereMaterial->setSpecularTexture("../Clove/res/Textures/DefaultTexture.png");

		sphereMaterial->setUniform1f("material.shininess", 32.0f);

		sphereMaterial->setUniform3f("light.ambient", glm::vec3(0.01f, 0.01f, 0.01f));
		sphereMaterial->setUniform3f("light.diffuse", glm::vec3(0.75f, 0.75f, 0.75f));
		sphereMaterial->setUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//MONKEY
		//monkeyMaterial->setSpecularTexture("../Clove/res/Textures/DefaultTexture.png");

		monkeyMaterial->setUniform1f("material.shininess", 32.0f);

		monkeyMaterial->setUniform3f("light.ambient", glm::vec3(0.01f, 0.01f, 0.01f));
		monkeyMaterial->setUniform3f("light.diffuse", glm::vec3(0.75f, 0.75f, 0.75f));
		monkeyMaterial->setUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));


		//LIGHT
		lightMaterial->setUniform1f("material.shininess", 32.0f);

		lightMaterial->setUniform3f("light.ambient", glm::vec3(1.0f));
		lightMaterial->setUniform3f("light.diffuse", glm::vec3(1.0f));
		lightMaterial->setUniform3f("light.specular", glm::vec3(1.0f));
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

		//CUBE
		cube.setRotation(glm::vec3(0.0f, 1.0f, 0.0f), rot);

		cubeMaterial->setUniform3f("light.position", lightCube.getPosition());
		cubeMaterial->setUniform3f("viewPos", cameraPosition);
		
		cube.draw(clv::Application::get().getWindow().getRenderer(), view, proj);

		//SPHERE
		sphere.setRotation(glm::vec3(0.0f, 1.0f, 0.0f), rot);

		sphereMaterial->setUniform3f("light.position", lightCube.getPosition());
		sphereMaterial->setUniform3f("viewPos", cameraPosition);

		sphere.draw(clv::Application::get().getWindow().getRenderer(), view, proj);

		//MONKEY
		monkey.setRotation(glm::vec3(0.0f, 1.0f, 0.5f), rot);

		monkeyMaterial->setUniform3f("light.position", lightCube.getPosition());
		monkeyMaterial->setUniform3f("viewPos", cameraPosition);

		monkey.draw(clv::Application::get().getWindow().getRenderer(), view, proj);
		
		//LIGHT
		lightMaterial->setUniform3f("viewPos", cameraPosition);

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