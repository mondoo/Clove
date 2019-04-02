#include <Clove.h>

//Clove
#include "Clove/Model.h"

#include "Clove/Rendering/Renderer.h"

#include "Clove/Window.h"

#include "Clove/Events/MouseEvent.h"

//Vendor
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	clv::Model cube;
	clv::Model lightCube;

	float r = 0.0f;
	float rot = 0.0f;
	float increment = 0.01f;

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -500.0f);
	glm::vec3 cubeScale = glm::vec3(100.0f, 100.0f, 100.0f);

	glm::vec3 cameraPosition = glm::vec3(0.0f, -30.0f, 0.0f);
	glm::vec3 cameraRotation = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 lightPos = glm::vec3(200.0f, 200.0f, -900.0f);
	glm::vec3 lightScale = glm::vec3(25.0f, 25.0f, 25.0f);

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
		cube = clv::Model("res/Objects/cube.obj");
		//cubeModel = clv::Model("res/Objects/cube.obj", "res/Textures/Zombie-32x32.png");

		lightCube = clv::Model("res/Objects/cube.obj");
	}

	virtual void onDetach() override{
		
	}

	virtual void onUpdate() override{
		clv::Application::get().getWindow().getRenderer().clear();

		float width = static_cast<float>(clv::Application::get().getWindow().getWidth());
		float height = static_cast<float>(clv::Application::get().getWindow().getHeight());

		//mvp matricies = model (the matrix of the thing we are rendering) | view (matrix of the camera) | projection (our projection matrix)
		glm::mat4 cubeTrans = glm::translate(glm::mat4(1.0f), cubePos);
		glm::mat4 cubeRot = glm::rotate(glm::mat4(1.0f), rot * 0.0174532925f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 cubeScale_mat = glm::scale(glm::mat4(1.0f), cubeScale);

		glm::mat4 lightTrans = glm::translate(glm::mat4(1.0f), lightPos);
		glm::mat4 lightRot = glm::mat4(1.0f);
		glm::mat4 lightScale_mat = glm::scale(glm::mat4(1.0f), lightScale);

		//Camera stuff
		glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
		
		cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.x = cos(glm::radians(yaw));
		cameraDirection.z = cos(glm::radians(yaw));

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
				
		/*glm::mat4 viewTrans = glm::translate(glm::mat4(1.0f), cameraPosition);
		glm::mat4 viewRot = glm::rotate(glm::mat4(1.0f), v_rot * 0.0174532925f, cameraRotation);
		glm::mat4 viewScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));*/

		const float camSpeed = 10.0f;

		if(clv::Input::isKeyPressed(clv::Key::W)){
			cameraPosition += camSpeed * cameraFront;
		} else if(clv::Input::isKeyPressed(clv::Key::S)){
			cameraPosition -= camSpeed * cameraFront;
		}

		if(clv::Input::isKeyPressed(clv::Key::Space)){
			cameraPosition += camSpeed * cameraUp;
		} else if(clv::Input::isKeyPressed(clv::Key::C)){
			cameraPosition -= camSpeed * cameraUp;
		}

		if(clv::Input::isKeyPressed(clv::Key::A)){
			cameraPosition -= camSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		} else if(clv::Input::isKeyPressed(clv::Key::D)){
			cameraPosition += camSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		}

		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		//~Camera stuff

		glm::mat4 proj = glm::perspective(45.0f, 16.0f / 9.0f, 1.0f, -1.0f);
		//glm::mat4 proj	= glm::ortho((width / -2), (width / 2), (height / -2), (height / 2)); //16:9
		/*glm::mat4 view = viewTrans * viewRot * viewScale*/;
		glm::mat4 cubeModel_mat = cubeTrans * cubeRot * cubeScale_mat;
		glm::mat4 lightModel_mat = lightTrans * lightRot * lightScale_mat;

		//opengl does the proj first
		//glm::mat4 cubeMVP = proj * view * cubeModel_mat;
		cube.setMVP(cubeModel_mat, view, proj);
		cube.setAmbientStrength(0.1f);
		cube.setLightPosition(lightPos);
		cube.setViewPosition(cameraPosition);
		cube.draw(clv::Application::get().getWindow().getRenderer());

		//glm::mat4 lightMVP = proj * view * lightModel_mat;
		lightCube.setMVP(lightModel_mat, view, proj);
		lightCube.draw(clv::Application::get().getWindow().getRenderer());

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
		static float outfloat;

		//Debug
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		ImGui::Text("Object:");
		ImGui::SliderFloat3("Object Transform", glm::value_ptr(cubePos), -1000.0f, 1000.0f);
		ImGui::SliderFloat3("Object Scale", glm::value_ptr(cubeScale), -300.0f, 300.0f);

		

		/*ImGui::Text("View:");
		ImGui::SliderFloat3("View Transform", glm::value_ptr(v_objTrans), -1000.0f, 1000.0f);
		ImGui::SliderFloat3("View Rotation", glm::value_ptr(v_rotAxis), -1.0f, 1.0f);
		ImGui::SliderFloat("View Rotation Amount", &v_rot_delta, 0.0f, 1.0f);*/
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