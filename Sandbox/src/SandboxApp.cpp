#include <Clove.hpp>

//Clove
#include "Clove/Object.hpp"
#include "Clove/Mesh.hpp"
#include "Clove/Rendering/API/Material.hpp"

#include "Clove/Camera.hpp"

#include "Clove/Window.hpp"

#include "Clove/Events/MouseEvent.hpp"

#include "Clove/Profiling/Timer.hpp"

#include "Clove/Rendering/Renderer.hpp"

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	std::array<clv::Object, 10> cubes = { clv::Object() };
	//std::shared_ptr<clv::Material> cubeMaterial;

	clv::Object sphere;
	std::shared_ptr<clv::Material> sphereMaterial;

	clv::Object monkey;
	std::shared_ptr<clv::Material> monkeyMaterial;

	clv::Object lightCube;
	std::shared_ptr<clv::Material> lightMaterial;

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
		std::for_each(cubes.begin(), cubes.end(), [](clv::Object& cube){
			std::shared_ptr<clv::Material> mat = std::make_shared<clv::Material>(clv::Material("res/Textures/container2.png"));
			
			mat->setSpecularTexture("res/Textures/container2_specular.png");
			mat->setUniform("material.shininess", 32.0f);
			
			cube.setMesh(std::make_shared<clv::Mesh>(clv::Mesh("res/Objects/cube.obj", mat)));
			cube.setScale(clv::math::Vector3f(100.0f, 100.0f, 100.0f));
		});
		
		cubes[0].setPosition(clv::math::Vector3f( 0,     0,	   0   ));
		cubes[1].setPosition(clv::math::Vector3f( 0,     2000, 3000));
		cubes[2].setPosition(clv::math::Vector3f( 750,  -2000, 3000));
		cubes[3].setPosition(clv::math::Vector3f( 0,     750,  1500));
		cubes[4].setPosition(clv::math::Vector3f( 0,    -750, -2000));
		cubes[5].setPosition(clv::math::Vector3f( 2000,  0,    1750));
		cubes[6].setPosition(clv::math::Vector3f(-1000, -2200, 500 ));
		cubes[7].setPosition(clv::math::Vector3f(-1500,  0,    1200));
		cubes[8].setPosition(clv::math::Vector3f( 0,     0,    1000));
		cubes[9].setPosition(clv::math::Vector3f( 0,     500, -1000));

		sphereMaterial = std::make_shared<clv::Material>(clv::Material());
		sphere.setMesh(std::make_shared<clv::Mesh>(clv::Mesh("res/Objects/sphere.obj", sphereMaterial)));
		sphere.setPosition(clv::math::Vector3f(-500, 0.0f, -900));
		sphere.setScale(clv::math::Vector3f(100.0f, 100.0f, 100.0f));

		monkeyMaterial = std::make_shared<clv::Material>(clv::Material());
		monkey.setMesh(std::make_shared<clv::Mesh>(clv::Mesh("res/Objects/monkey.obj", monkeyMaterial)));
		monkey.setPosition(clv::math::Vector3f(500, 0.0f, -900));
		monkey.setScale(clv::math::Vector3f(100.0f, 100.0f, 100.0f));

		lightMaterial = std::make_shared<clv::Material>(clv::Material());
		lightCube.setMesh(std::make_shared<clv::Mesh>(clv::Mesh("res/Objects/cube.obj", lightMaterial)));
		lightCube.setPosition(clv::math::Vector3f(0, 200, -900));
		lightCube.setScale(clv::math::Vector3f(25.0f, 25.0f, 25.0f));

		//Material shiz
		//SPHERE
		//sphereMaterial->setSpecularTexture("../Clove/res/Textures/DefaultTexture.png");

		sphereMaterial->setUniform("material.shininess", 32.0f);

		//MONKEY
		//monkeyMaterial->setSpecularTexture("../Clove/res/Textures/DefaultTexture.png");

		monkeyMaterial->setUniform("material.shininess", 32.0f);


		//LIGHT
		lightMaterial->setUniform("material.shininess", 32.0f);
	}

	virtual void onDetach() override{
		
	}

	virtual void onUpdate() override{
		clv::Camera& cam = clv::Application::get().getWindow().getCurrentCamera();
		
		//TODO: Needs to happen here until the scene thing has been updated properly
		clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.ambient", clv::math::Vector3f(0.01f, 0.01f, 0.01f));
		clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.diffuse", clv::math::Vector3f(0.75f, 0.75f, 0.75f));
		clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.specular", clv::math::Vector3f(1.0f, 1.0f, 1.0f));

		clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.position", cam.getPosition());
		clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.direction", cam.getFront());
		clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.cutOff", clv::math::cos(clv::math::asRadians(12.5f)));
		clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.outerCutOff", clv::math::cos(clv::math::asRadians(17.5f)));

		//these will change for how much of a distance we want the light to cover (could be converted from an 'intesity' variable)

		//clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.constant",	1.0f);
		//clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.linear",	0.0014f);
		//clv::Application::get().getWindow().getRenderer().setGlobalShaderUniform("light.quadratic", 0.000007f);
		//

		lightMaterial->setUniform("light.ambient", clv::math::Vector3f(1.0f));
		lightMaterial->setUniform("light.diffuse", clv::math::Vector3f(1.0f));
		lightMaterial->setUniform("light.specular", clv::math::Vector3f(1.0f));

		const float camSpeed = 10.0f;


		cam.update(pitch, yaw);

		clv::math::Vector3f cameraPosition = cam.getPosition();

		const clv::math::Vector3f front = cam.getFront();
		if(clv::Input::isKeyPressed(clv::Key::W)){
			cameraPosition += camSpeed * front;
		} else if(clv::Input::isKeyPressed(clv::Key::S)){
			cameraPosition -= camSpeed * front;
		}

		const clv::math::Vector3f up = cam.getUp();
		if(clv::Input::isKeyPressed(clv::Key::Space)){
			cameraPosition += camSpeed * up;
		} else if(clv::Input::isKeyPressed(clv::Key::C)){
			cameraPosition -= camSpeed * up;
		}

		const clv::math::Vector3f right = cam.getRight();
		if(clv::Input::isKeyPressed(clv::Key::A)){
			cameraPosition -= camSpeed * right;
		} else if(clv::Input::isKeyPressed(clv::Key::D)){
			cameraPosition += camSpeed * right;
		}

		cam.setPosition(cameraPosition);

		//CUBE
		for(int i = 0; i < cubes.size(); i++){
			if(i == 0){
				cubes[i].setRotation(clv::math::Vector3f(0, 1, 0), 1);
			} else{
				cubes[i].setRotation(clv::math::Vector3f(i * 0.25f, i, i * 0.75f), rot);
			}
			cubes[i].draw(clv::Application::get().getWindow());
		}

		//SPHERE
		sphere.setRotation(clv::math::Vector3f(0.0f, 1.0f, 0.0f), rot);
		sphere.draw(clv::Application::get().getWindow());

		//MONKEY
		monkey.setRotation(clv::math::Vector3f(0.0f, 1.0f, 0.5f), rot);
		monkey.draw(clv::Application::get().getWindow());

		//LIGHT
		lightCube.draw(clv::Application::get().getWindow());

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