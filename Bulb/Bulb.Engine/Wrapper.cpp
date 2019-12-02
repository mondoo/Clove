#include "Wrapper.hpp"

//Other includes
#include "Clove/Core/Layer.hpp"
#include "Clove/Core/ECS/Manager.hpp"
#include "Clove/Core/ECS/Entity.hpp"
#include "Clove/Core/ECS/3D/Components/TransformComponent.hpp"
#include "Clove/Core/ECS/3D/Components/MeshComponent.hpp"
#include "Clove/Core/ECS/3D/Components/CameraComponent.hpp"
#include "Clove/Core/Graphics/Renderables/Mesh.hpp"
#include "Clove/Core/Graphics/Material.hpp"
#include "Clove/Core/Graphics/MaterialInstance.hpp"

//Bulb stuff
#include "BulbApplication.hpp"
#include "BulbWindow.hpp"

//Temp layer to draw something
class TestLayer : public clv::Layer{
	//VARIABLES
private:
	clv::ecs::Entity cubeEnt;
	clv::ecs::Entity camEnt;

	//FUNCTIONS
public:
	virtual void onAttach() override{
		auto cubeMaterial = std::make_shared<clv::gfx::Material>();
		cubeMaterial->setData(clv::gfx::BBP_Colour, clv::mth::vec4f{ 0.4f, 0.4f, 0.4f, 1.0f }, clv::gfx::ShaderType::Pixel);
		cubeMaterial->setData(clv::gfx::BBP_MaterialData, clv::gfx::MaterialData{ 32.0f }, clv::gfx::ShaderType::Pixel);

		auto inst = cubeMaterial->createInstance();
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", inst); //NOTE: Manually copied to the correct folder for now

		cubeEnt = clv::plt::Application::get().getManager().createEntity();
		cubeEnt.addComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ 10.0f, 0.0f, 0.0f });
		cubeEnt.addComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);


		camEnt = clv::plt::Application::get().getManager().createEntity();
		camEnt.addComponent<clv::ecs::_3D::TransformComponent>();
		camEnt.addComponent<clv::ecs::_3D::CameraComponent>(clv::gfx::Viewport{ 0, 0, 800, 400 }); //Any old viewport for now
	}

	virtual void onDetach() override{
	
	}

	virtual void onUpdate(clv::utl::DeltaTime deltaTime) override{
		static float rotDelta = 0.0f;
		const float rotSpeed = 1.5f;

		cubeEnt.getComponent<clv::ecs::_3D::TransformComponent>()->setRotation(clv::mth::asQuaternion(rotDelta, clv::mth::vec3f{ 0.0f, 1.0f, 0.0f }));

		rotDelta += rotSpeed * deltaTime;
	}
};

void Bulb::Engine::Wrapper::OpenClove(System::IntPtr hWnd, int width, int height){
	app = new BulbApplication(hWnd, width, height);
	
	app->start();

	app->pushLayer(std::make_shared<TestLayer>());

	while(app->getState() == clv::plt::ApplicationState::running){
		app->update();
	}

	delete app;
	app = nullptr;
}

void Bulb::Engine::Wrapper::StopClove(){
	app->stop();
}
