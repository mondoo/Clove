#include <Clove.hpp>

//Clove
#include "Clove/Scene/CameraSceneNode.hpp"
#include "Clove/Scene/Lights/PointLightSceneNode.hpp"
#include "Clove/Scene/Lights/DirectionalLightSceneNode.hpp"

#include "Clove/Platform/Window.hpp"

#include "Clove/Events/MouseEvent.hpp"

#include "Clove/Profiling/Timer.hpp"

#include "Clove/Scene/Scene.hpp"


class ExampleLayer : public clv::Layer{
	//VARIABLES
private:
	std::shared_ptr<clv::scene::CameraSceneNode> cam;

	std::shared_ptr<clv::scene::MeshSceneNode> cube;

	float rotDelta = 0.0f;

	//FUNCTIONS
public:
	ExampleLayer()
		: Layer("Sanbox render test"){
	}

	virtual void onAttach() override{
		/*std::shared_ptr<clv::scene::Scene> scene = clv::Application::get().getScene();

		cam = scene->createCameraSceneNode();

		cube = scene->createMeshSceneNode();
		cube->setMesh("res/Objects/cube.obj");
		cube->setMaterial(std::make_shared<clv::Material>());
		cube->setPosition(clv::math::Vector3f(0.0f, 0.0f, -5.0f));*/
	}

	virtual void onDetach() override{

	}

	virtual void onUpdate() override{
		/*cube->setRotation({ clv::math::Vector3f(0.0f, 1.0f, 0.0f), rotDelta});
		rotDelta += 0.01f;*/
	}

	virtual void onImGuiRender() override{

	}

	virtual void onEvent(clv::Event& e) override{
		
	}
};

class SandBox : public clv::Application{
public:
	SandBox(){
		//pushLayer(std::make_shared<ExampleLayer>(ExampleLayer()));
	}
	~SandBox(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new SandBox;
}