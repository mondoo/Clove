#include <Clove.hpp>

//Clove
#include "Clove/Scene/MeshSceneNode.hpp"
#include "Clove/Rendering/API/Material.hpp"

#include "Clove/Scene/CameraSceneNode.hpp"
#include "Clove/Scene/Lights/PointLightSceneNode.hpp"
#include "Clove/Scene/Lights/DirectionalLightSceneNode.hpp"

#include "Clove/Window.hpp"

#include "Clove/Events/MouseEvent.hpp"

#include "Clove/Profiling/Timer.hpp"

#include "Clove/Scene/Scene.hpp"

//REMOVE
#include "Clove/Rendering/Renderer.hpp"
//

class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	std::array<std::shared_ptr<clv::scene::MeshSceneNode>, 10> cubes;
	//std::shared_ptr<clv::Material> cubeMaterial;

	std::shared_ptr<clv::scene::MeshSceneNode> sphere;
	std::shared_ptr<clv::Material> sphereMaterial;

	std::shared_ptr<clv::scene::MeshSceneNode> monkey;
	std::shared_ptr<clv::Material> monkeyMaterial;

	std::array<std::shared_ptr<clv::scene::PointLightSceneNode>, 4> lights;
	std::array<std::shared_ptr<clv::scene::MeshSceneNode>, 4> lightCubes;
	std::shared_ptr<clv::scene::DirectionalLightSceneNode> dirLight;
	//std::shared_ptr<clv::Material> lightMaterial;

	std::shared_ptr<clv::scene::CameraSceneNode> cam;

	float rot = 0.0f;
	float increment = 0.01f;

	float yaw = 0.0f;
	float pitch = 0.0f;

	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;

	bool firstMouse = true;

	//Testies
	std::shared_ptr<clv::scene::MeshSceneNode> parentCube;
	std::shared_ptr<clv::scene::MeshSceneNode> childCube;

	clv::math::Vector3f parentPos = clv::math::Vector3f(2.0f, 0.0f, 0.0f);
	clv::math::Vector3f childPos = clv::math::Vector3f(2.0f, 0.0f, 0.0f);

	clv::math::Vector3f parentRotV = clv::math::Vector3f(1.0f);
	float parentRotA = 0.0f;
	clv::math::Vector3f childRotV = clv::math::Vector3f(1.0f);
	float childRotA = 0.0f;

	clv::math::Vector3f parentScale = clv::math::Vector3f(1.0f);
	clv::math::Vector3f childScale = clv::math::Vector3f(1.0f);

	//~

	//FUNCTIONS
public:
	ExampleLayer()
		: Layer("Sanbox render test"){
	}

	virtual void onAttach() override{
		std::shared_ptr<clv::scene::Scene> scene = clv::Application::get().getScene();
		cam = scene->createCameraSceneNode();

		std::for_each(cubes.begin(), cubes.end(), [&](std::shared_ptr<clv::scene::MeshSceneNode>& cube){
			cube = scene->createMeshSceneNode();
	
			std::shared_ptr<clv::Material> mat = std::make_shared<clv::Material>(clv::Material("res/Textures/container2.png"));

			mat->setSpecularTexture("res/Textures/container2_specular.png");
			mat->setUniform("material.shininess", 32.0f);

			cube->setMesh("res/Objects/cube.obj");
			cube->setMaterial(mat);
		});

		cubes[0]->setPosition(clv::math::Vector3f(0, 0, 0));
		cubes[1]->setPosition(clv::math::Vector3f(0, 20, 30));
		cubes[2]->setPosition(clv::math::Vector3f(7.5f, -20, 30));
		cubes[3]->setPosition(clv::math::Vector3f(0, 7.5f, 15));
		cubes[4]->setPosition(clv::math::Vector3f(0, -7.5f, -20));
		cubes[5]->setPosition(clv::math::Vector3f(20, 0, 1.75f));
		cubes[6]->setPosition(clv::math::Vector3f(-10, -22, 5));
		cubes[7]->setPosition(clv::math::Vector3f(-15, 0, 12));
		cubes[8]->setPosition(clv::math::Vector3f(0, 0, 10));
		cubes[9]->setPosition(clv::math::Vector3f(0, 5, -10));

		sphereMaterial = std::make_shared<clv::Material>(clv::Material());
		sphere = scene->createMeshSceneNode();
		sphere->setMesh("res/Objects/sphere.obj");
		sphere->setMaterial(sphereMaterial);
		sphere->setPosition(clv::math::Vector3f(-5, 0.0f, -9));

		monkeyMaterial = std::make_shared<clv::Material>(clv::Material());
		monkey = scene->createMeshSceneNode();
		monkey->setMesh("res/Objects/monkey.obj");
		monkey->setMaterial(monkeyMaterial);
		monkey->setPosition(clv::math::Vector3f(5, 0.0f, -9));

		for(int i = 0; i < lights.size(); ++i){
			lights[i] = scene->createPointLightSceneNode();
			lightCubes[i] = scene->createMeshSceneNode(lights[i]);

			std::shared_ptr<clv::Material> lightMaterial = std::make_shared<clv::Material>(clv::Material(ShaderType::light));

			lightCubes[i]->setMesh("res/Objects/cube.obj");
			lightCubes[i]->setMaterial(lightMaterial);
			lightCubes[i]->setScale(clv::math::Vector3f(0.25f, 0.25f, 0.25f));
		}

		lights[0]->setPosition(clv::math::Vector3f(0, 2, -9));
		lights[1]->setPosition(clv::math::Vector3f(1, 1, 1));
		lights[2]->setPosition(clv::math::Vector3f(0, -10, 0));
		lights[3]->setPosition(clv::math::Vector3f(5, 7.5f, 0));

		dirLight = scene->createDirectionalLightSceneNode();

		//Material shiz
		//SPHERE
		sphereMaterial->setUniform("material.shininess", 32.0f);

		//MONKEY
		monkeyMaterial->setUniform("material.shininess", 32.0f);

		parentCube = scene->createMeshSceneNode();
		parentCube->setMesh("res/Objects/cube.obj");
		parentCube->setMaterial(std::make_shared<clv::Material>());

		childCube = scene->createMeshSceneNode(parentCube);
		childCube->setMesh("res/Objects/cube.obj");
		childCube->setMaterial(std::make_shared<clv::Material>());
	}

	virtual void onDetach() override{

	}

	virtual void onUpdate() override{
		const float camSpeed = 10.0f;

		//cam.update(pitch, yaw);

		clv::math::Vector3f cameraPosition = cam->getPosition();

		const clv::math::Vector3f front = cam->getFront();
		if(clv::Input::isKeyPressed(clv::Key::W)){
			cameraPosition += camSpeed * front;
		} else if(clv::Input::isKeyPressed(clv::Key::S)){
			cameraPosition -= camSpeed * front;
		}

		const clv::math::Vector3f up = cam->getUp();
		if(clv::Input::isKeyPressed(clv::Key::Space)){
			cameraPosition += camSpeed * up;
		} else if(clv::Input::isKeyPressed(clv::Key::C)){
			cameraPosition -= camSpeed * up;
		}

		const clv::math::Vector3f right = cam->getRight();
		if(clv::Input::isKeyPressed(clv::Key::A)){
			cameraPosition -= camSpeed * right;
		} else if(clv::Input::isKeyPressed(clv::Key::D)){
			cameraPosition += camSpeed * right;
		}

		cam->setPosition(cameraPosition);

		//CUBE
		for(int i = 0; i < cubes.size(); i++){
			if(i == 0){
				cubes[i]->setRotation(std::make_pair(clv::math::Vector3f(0, 1, 0), 0.0f));
			} else{
				cubes[i]->setRotation(std::make_pair(clv::math::Vector3f(i * 0.25f, i, i * 0.75f), rot));
			}
			cubes[i]->draw(clv::Application::get().getRenderer());
		}

		//SPHERE
		sphere->setRotation(std::make_pair(clv::math::Vector3f(0.0f, 1.0f, 0.0f), rot));
		sphere->draw(clv::Application::get().getRenderer());

		//MONKEY
		monkey->setRotation(std::make_pair(clv::math::Vector3f(0.0f, 1.0f, 0.5f), rot));
		monkey->draw(clv::Application::get().getRenderer());

		//LIGHT
		for(int i = 0; i < lights.size(); ++i){
			lights[i]->setRotation(std::make_pair(clv::math::Vector3f(0, 1, 0), 1.0f));
			lightCubes[i]->draw(clv::Application::get().getRenderer());
		}

		rot += clv::math::asRadians(0.5f);

		if(clv::Input::isKeyPressed(clv::Key::Escape)){
			clv::Application::get().stop();
		}

		//TESTIES
		parentCube->setPosition(parentPos);
		parentCube->setRotation(std::make_pair(parentRotV, clv::math::asRadians(parentRotA)));
		parentCube->setScale(parentScale);
		parentCube->draw(clv::Application::get().getRenderer());
		
		childCube->setPosition(childPos);
		childCube->setRotation(std::make_pair(childRotV, clv::math::asRadians(childRotA)));
		childCube->setScale(childScale);
		childCube->draw(clv::Application::get().getRenderer());
	}

	virtual void onImGuiRender() override{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
		ImGui::Spacing();

		ImGui::SliderFloat3("Parent Cube Position", clv::math::valuePtr(parentPos), -50, 50);
		ImGui::SliderFloat3("Parent Cube Rotation Vector", clv::math::valuePtr(parentRotV), 0, 1);
		ImGui::SliderFloat("Parent Cube Rotation Angle", &parentRotA, 0, 360);
		ImGui::SliderFloat3("Parent Cube Scale", clv::math::valuePtr(parentScale), 1, 2);
		
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SliderFloat3("Child Cube Position", clv::math::valuePtr(childPos), -50, 50);
		ImGui::SliderFloat3("Child Cube Rotation Vector", clv::math::valuePtr(childRotV), 0, 1);
		ImGui::SliderFloat("Child Cube Rotation Angle", &childRotA, 0, 360);
		ImGui::SliderFloat3("Child Cube Scale", clv::math::valuePtr(childScale), 1, 2);
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