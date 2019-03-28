#include <Clove.h>

//TODO: need abstraction!

//Clove
#include "Clove/Model.h"

#include "Clove/Rendering/Renderer.h"

//Vendor
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	clv::Model cubeModel;

	float r = 0.0f;
	float rot = 0.0f;
	float increment = 0.01f;

	glm::vec3 v_objTrans = glm::vec3(0.0f, 0.0f, -500.0f);
	glm::vec3 v_objScale = glm::vec3(100.0f, 100.0f, 100.0f);

	glm::vec3 v_viewTrans = glm::vec3(0.0f, -30.0f, 0.0f);
	glm::vec3 v_rotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	float v_rot = 0.0f;
	float v_rot_delta = 0.5f;

	//FUNCTIONS
public:
	ExampleLayer() 
		: Layer("Sanbox render test"){
	}

	virtual void onAttach() override{
		//cubeModel = clv::Model("res/Objects/cube.obj");
		cubeModel = clv::Model("res/Objects/cube.obj", "res/Textures/Zombie-32x32.png");
	}

	virtual void onDetach() override{
		
	}

	virtual void onUpdate() override{
		if(clv::Input::isKeyPressed(clv::Key::W)){
			v_viewTrans.z += 10.0f;
		}else if(clv::Input::isKeyPressed(clv::Key::S)){
			v_viewTrans.z -= 10.0f;
		}

		if(clv::Input::isKeyPressed(clv::Key::A)){
			v_rot -= v_rot_delta;
		} else if(clv::Input::isKeyPressed(clv::Key::D)){
			v_rot += v_rot_delta;
		}
		
		clv::Application::get().getWindow().getRenderer().clear();


		float width = static_cast<float>(clv::Application::get().getWindow().getWidth());
		float height = static_cast<float>(clv::Application::get().getWindow().getHeight());

		//mvp matricies = model (the matrix of the thing we are rendering) | view (matrix of the camera) | projection (our projection matrix)
		glm::mat4 objTrans = glm::translate(glm::mat4(1.0f), v_objTrans);
		glm::mat4 objRot = glm::rotate(glm::mat4(1.0f), rot * 0.0174532925f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 objScale = glm::scale(glm::mat4(1.0f), v_objScale);

		glm::mat4 viewTrans = glm::translate(glm::mat4(1.0f), v_viewTrans);
		glm::mat4 viewRot = glm::rotate(glm::mat4(1.0f), v_rot * 0.0174532925f, v_rotAxis);
		glm::mat4 viewScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 proj = glm::perspective(45.0f, 16.0f / 9.0f, 1.0f, -1.0f);
		//glm::mat4 proj	= glm::ortho((width / -2), (width / 2), (height / -2), (height / 2)); //16:9
		glm::mat4 view = viewTrans * viewRot * viewScale;
		glm::mat4 model = objTrans * objRot * objScale;

		//opengl does the proj first
		glm::mat4 mvp = proj * view * model;
		
		cubeModel.setMVP(mvp);
		cubeModel.draw(clv::Application::get().getWindow().getRenderer());

		if(r > 1.0f || r < 0.0f){
			increment = -increment;
		}
		r += increment;
		rot += 0.5f;
	}

	virtual void onImGuiRender() override{
		static float outfloat;

		//Debug
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		ImGui::Text("Object:");
		ImGui::SliderFloat3("Object Transform", glm::value_ptr(v_objTrans), -1000.0f, 1000.0f);
		ImGui::SliderFloat3("Object Scale", glm::value_ptr(v_objScale), -300.0f, 300.0f);

		

		/*ImGui::Text("View:");
		ImGui::SliderFloat3("View Transform", glm::value_ptr(v_objTrans), -1000.0f, 1000.0f);
		ImGui::SliderFloat3("View Rotation", glm::value_ptr(v_rotAxis), -1.0f, 1.0f);
		ImGui::SliderFloat("View Rotation Amount", &v_rot_delta, 0.0f, 1.0f);*/
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