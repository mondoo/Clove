#include "TestLayer.hpp"

#include "Core/Application.hpp"
#include "Core/Platform/Window.hpp"
#include "Core/ECS/Manager.hpp"
#include "Core/ECS/3D/Components/MeshComponent.hpp"
#include "Core/ECS/2D/Components/SpriteComponent.hpp"
#include "Core/ECS/2D/Components/TransformComponent.hpp"
#include "Core/ECS/2D/Components/RigidBodyComponent.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"
#include "Core/ECS/3D/Components/CameraComponent.hpp"
#include "Core/ECS/3D/Components/LightComponent.hpp"
#include "Core/ECS/3D/Components/RigidBodyComponent.hpp"
#include "Core/ECS/Audio/Components/AudioComponent.hpp"
#include "Core/ECS/UI/Components/TextComponent.hpp"
#include "Core/Maths/Vector.hpp"
#include "Core/Input/Input.hpp"
#include "Core/Utils/DeltaTime.hpp"
#include "Core/Graphics/BindableFactory.hpp"
#include "Core/Graphics/Bindables/Texture.hpp"
#include "Core/Graphics/RenderTarget.hpp"
#include "Core/Graphics/Mesh.hpp"
#include "Core/Graphics/Sprite.hpp"
#include "Core/Graphics/Material.hpp"

#include <ostream>

TestLayer::TestLayer()
	: Layer("Example Layer"){
}

void TestLayer::onAttach(){
	ent1 = clv::Application::get().getManager().createEntity();
	ent1.addComponent<clv::ecs::_3D::MeshComponent>();
	ent1.addComponent<clv::ecs::_3D::TransformComponent>();

	ent2 = clv::Application::get().getManager().createEntity();
	ent2.addComponent<clv::ecs::_3D::MeshComponent>();
	ent2.addComponent<clv::ecs::_3D::TransformComponent>();

	ent3 = clv::Application::get().getManager().createEntity();
	ent3.addComponent<clv::ecs::_3D::MeshComponent>();
	ent3.addComponent<clv::ecs::_3D::TransformComponent>();

	//rtEnt = clv::Application::get().getManager().createEntity<clv::ecs::_2D::SpriteComponent, clv::ecs::_2D::TransformComponent>();

	sprtEnt1 = clv::Application::get().getManager().createEntity();
	sprtEnt1.addComponent<clv::ecs::_2D::SpriteComponent>();
	sprtEnt1.addComponent<clv::ecs::_2D::TransformComponent>();

	sprtEnt2 = clv::Application::get().getManager().createEntity();
	sprtEnt2.addComponent<clv::ecs::_2D::SpriteComponent>();
	sprtEnt2.addComponent<clv::ecs::_2D::TransformComponent>();
	
	lght1 = clv::Application::get().getManager().createEntity();
	lght1.addComponent<clv::ecs::_3D::MeshComponent>();
	lght1.addComponent<clv::ecs::_3D::LightComponent>();
	lght1.addComponent<clv::ecs::_3D::TransformComponent>();

	lght2 = clv::Application::get().getManager().createEntity();
	lght2.addComponent<clv::ecs::_3D::MeshComponent>();
	lght2.addComponent<clv::ecs::_3D::LightComponent>();
	lght2.addComponent<clv::ecs::_3D::TransformComponent>();
	
	cam = clv::Application::get().getManager().createEntity();
	cam.addComponent<clv::ecs::_3D::CameraComponent>();
	cam.addComponent<clv::ecs::_3D::TransformComponent>();

	sound = clv::Application::get().getManager().createEntity();
	sound.addComponent<clv::ecs::aud::AudioComponent>();

	bigBoy = clv::Application::get().getManager().createEntity();
	bigBoy.addComponent<clv::ecs::_3D::MeshComponent>();
	bigBoy.addComponent<clv::ecs::_3D::TransformComponent>();

	auto cubeMaterial = std::make_shared<clv::gfx::Material>(clv::gfx::ShaderStyle::Lit);
	cubeMaterial->setAlbedoTexture("res/Textures/container2.png");
	cubeMaterial->setSpecularTexture("res/Textures/container2_specular.png");
	cubeMaterial->setData(clv::gfx::BBP_MaterialData, clv::gfx::MaterialData{ 32.0f }, clv::gfx::ShaderType::Pixel);

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		ent1.getComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		ent2.getComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		ent3.getComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		bigBoy.getComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
	}

	ent1.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ 0.0f, 0.0f, 0.0f });
	ent2.getComponent<clv::ecs::_3D::TransformComponent>()->setLocalPosition({ 0.0f, 0.0f, 3.0f });
	ent3.getComponent<clv::ecs::_3D::TransformComponent>()->setLocalPosition({ 0.0f, 3.0f, 0.0f });

	ent1.getComponent<clv::ecs::_3D::TransformComponent>()->addChild(ent2.getComponent<clv::ecs::_3D::TransformComponent>());
	ent2.getComponent<clv::ecs::_3D::TransformComponent>()->addChild(ent3.getComponent<clv::ecs::_3D::TransformComponent>());

	bigBoy.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ 20.0f, 0.0f, 0.0f });
	bigBoy.getComponent<clv::ecs::_3D::TransformComponent>()->setScale({ 10.0f, 10.0f, 10.0f });

	//clv::Application::get().getManager().destroyEntity(ent2.getID());

	/*auto rtTexture = clv::gfx::BindableFactory::createTexture(clv::Application::get().getWindow().getWidth(), clv::Application::get().getWindow().getHeight(), clv::gfx::TextureUsage::RenderTarget, clv::gfx::TBP_Albedo);
	auto sprite = std::make_shared<clv::gfx::Sprite>(rtTexture);
	rtEnt.getComponent<clv::ecs::_2D::SpriteComponent>()->setSprite(sprite);
	rtEnt.getComponent<clv::ecs::_2D::TransformComponent>()->setScale(clv::math::Vector2f(clv::Application::get().getWindow().getWidth() / 2, clv::Application::get().getWindow().getHeight() / 2));

	auto renderTarget = clv::gfx::RenderTarget::createRenderTarget(*rtTexture);
	clv::gfx::Renderer::setRenderTarget(renderTarget);*/

	{
		auto sprite = std::make_shared<clv::gfx::Sprite>("res/Textures/Zombie-32x32.png");
		sprtEnt1.getComponent<clv::ecs::_2D::SpriteComponent>()->setSprite(sprite);
		sprtEnt1.getComponent<clv::ecs::_2D::TransformComponent>()->setScale(clv::math::Vector2f(20.0f, 20.0f));
	}

	{
		auto sprite = std::make_shared<clv::gfx::Sprite>("res/Textures/Zombie-32x32.png");
		sprtEnt2.getComponent<clv::ecs::_2D::SpriteComponent>()->setSprite(sprite);
		sprtEnt2.getComponent<clv::ecs::_2D::TransformComponent>()->setLocalPosition(clv::math::Vector2f(0.0f, 2.0f));
		sprtEnt1.getComponent<clv::ecs::_2D::TransformComponent>()->setScale(clv::math::Vector2f(20.0f, 20.0f));
	}

	sprtEnt1.getComponent<clv::ecs::_2D::TransformComponent>()->addChild(sprtEnt2.getComponent<clv::ecs::_2D::TransformComponent>());

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		lght1.getComponent<clv::ecs::_3D::TransformComponent>()->setScale({ 0.25f, 0.25f, 0.25f });
		lght1.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ -10.0f, 0.0f, 0.0f });
		lght1.getComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		lght2.getComponent<clv::ecs::_3D::TransformComponent>()->setScale({ 0.25f, 0.25f, 0.25f });
		lght2.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ -10.0f, -5.0f, 0.0f });
		lght2.getComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
	}

	{
		auto soundBuffer = clv::aud::Sound("res/Audio/Test.wav");
		sound.getComponent<clv::ecs::aud::AudioComponent>()->setSound(soundBuffer);
		//sound.getComponent<clv::ecs::aud::AudioComponent>()->play();
	}

	//Fonts
	{
		auto font = clv::ui::Font("res/Fonts/Roboto/Roboto-Black.ttf");

		fontEnt = clv::Application::get().getManager().createEntity();
		fontEnt.addComponent<clv::ecs::ui::TextComponent>(font);
		fontEnt.addComponent<clv::ecs::_2D::TransformComponent>()->setPosition(clv::math::Vector2f{-550.0, 300.0f});

		fontEnt.getComponent<clv::ecs::ui::TextComponent>()->setText("Hello, World!");
		fontEnt.getComponent<clv::ecs::ui::TextComponent>()->setSize(72);

		fpsEnt = clv::Application::get().getManager().createEntity();
		fpsEnt.addComponent<clv::ecs::ui::TextComponent>(font);
		fpsEnt.addComponent<clv::ecs::_2D::TransformComponent>()->setPosition(clv::math::Vector2f{-550.0, 100.0f});

		fpsEnt.getComponent<clv::ecs::ui::TextComponent>()->setText("not set :(");
		fpsEnt.getComponent<clv::ecs::ui::TextComponent>()->setSize(30);
	}

	//Physics
	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());

		rigidBody1 = clv::Application::get().getManager().createEntity();
		rigidBody1.addComponent<clv::ecs::_3D::TransformComponent>()->setPosition(clv::math::Vector3f{ 0.0f, 10.0f, 10.0f });
		rigidBody1.addComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
		rigidBody1.addComponent<clv::ecs::_3D::RigidBodyComponent>(1.0f, true, clv::math::Vector3f{ 1.0f, 1.0f, 1.0f });

		rigidBody1.getComponent<clv::ecs::_3D::TransformComponent>()->setRotation(clv::math::asQuaternion(13.0f, clv::math::Vector3f{ 0.0f, 0.0f, 0.5f }));
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());

		rigidBody2 = clv::Application::get().getManager().createEntity();
		rigidBody2.addComponent<clv::ecs::_3D::TransformComponent>()->setPosition(clv::math::Vector3f{ 0.0f, 30.0f, 10.0f });
		rigidBody2.addComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
		rigidBody2.addComponent<clv::ecs::_3D::RigidBodyComponent>(1.0f, false, clv::math::Vector3f{ 1.0f, 1.0f, 1.0f });

		rigidBody2.getComponent<clv::ecs::_3D::TransformComponent>()->setRotation(clv::math::asQuaternion(13.0f, clv::math::Vector3f{ 1.0f, 0.0f, 0.0f }));
	}

	{
		auto sprite = std::make_shared<clv::gfx::Sprite>("res/Textures/Zombie-32x32.png");

		rigidSprite1 = clv::Application::get().getManager().createEntity();
		rigidSprite1.addComponent<clv::ecs::_2D::SpriteComponent>()->setSprite(sprite);
		rigidSprite1.addComponent<clv::ecs::_2D::TransformComponent>()->setScale(clv::math::Vector2f(20.0f, 20.0f));
		rigidSprite1.addComponent<clv::ecs::_2D::RigidBodyComponent>(1.0f, true, clv::math::Vector2f{ 20.0f, 20.0f });

		rigidSprite1.getComponent<clv::ecs::_2D::TransformComponent>()->setPosition(clv::math::Vector2f{ -100.0f, 0.0f });
		rigidSprite1.getComponent<clv::ecs::_2D::TransformComponent>()->setRotation(clv::math::asRadians(45.0f));
	}

	{
		auto sprite = std::make_shared<clv::gfx::Sprite>("res/Textures/Zombie-32x32.png");

		rigidSprite2 = clv::Application::get().getManager().createEntity();
		rigidSprite2.addComponent<clv::ecs::_2D::SpriteComponent>()->setSprite(sprite);
		rigidSprite2.addComponent<clv::ecs::_2D::TransformComponent>()->setScale(clv::math::Vector2f(20.0f, 20.0f));
		rigidSprite2.addComponent<clv::ecs::_2D::RigidBodyComponent>(1.0f, false, clv::math::Vector2f{ 20.0f, 20.0f });

		rigidSprite2.getComponent<clv::ecs::_2D::TransformComponent>()->setPosition(clv::math::Vector2f{ -125.0f, 200.0f });
	}

	CLV_LOG_DEBUG("Testlayer has done a thing!");
}

void TestLayer::onDetach(){

}

void TestLayer::onUpdate(clv::utl::DeltaTime deltaTime){
	const float camSpeed = 15.0f;
	const float camDelta = camSpeed * deltaTime;
	const float rotSpeed = 1.5f;
	//const float deltaTime = deltaTime;

	float s = 6 * deltaTime;

	clv::math::Vector3f cameraPosition = cam.getComponent<clv::ecs::_3D::TransformComponent>()->getPosition();

	//cam->updateFront(pitch, yaw); //TODO: proper first person implementation

	const clv::math::Vector3f front = cam.getComponent<clv::ecs::_3D::CameraComponent>()->getFront();
	if(clv::input::isKeyPressed(clv::Key::W)){
		cameraPosition += front * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::S)){
		cameraPosition -= front * camDelta;
	}

	const clv::math::Vector3f up = cam.getComponent<clv::ecs::_3D::CameraComponent>()->getUp();
	if(clv::input::isKeyPressed(clv::Key::Space)){
		cameraPosition += up * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::C)){
		cameraPosition -= up * camDelta;
	}

	const clv::math::Vector3f right = cam.getComponent<clv::ecs::_3D::CameraComponent>()->getRight();
	if(clv::input::isKeyPressed(clv::Key::A)){
		cameraPosition -= right * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::D)){
		cameraPosition += right * camDelta;
	}

	if(clv::input::isKeyPressed(clv::Key::Q)){
		yaw -= camDelta * 10.0f;
	} else if(clv::input::isKeyPressed(clv::Key::E)){
		yaw += camDelta * 10.0f;
	}

	const float radius = 6.0f;

	cam.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition(cameraPosition);
	cam.getComponent<clv::ecs::_3D::CameraComponent>()->updateFront(0.0f, yaw);

	ent1.getComponent<clv::ecs::_3D::TransformComponent>()->setRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 0.0f, 1.0f, 0.0f }));
	ent2.getComponent<clv::ecs::_3D::TransformComponent>()->setLocalRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 0.0f, 0.0f, 1.0f }));
	ent3.getComponent<clv::ecs::_3D::TransformComponent>()->setLocalRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 1.0f, 0.0f, 0.0f }));

	ent1.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ cos(rotDelta) * radius, 0.0f, 0.0f });

	//lght1.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ cos(rotDelta * 1.5f) * radius * 2.0f, 0.0f, sin(rotDelta * 1.5f) * radius * 2.0f });
	//lght2.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ cos(rotDelta) * radius * 2.0f, sin(rotDelta) * radius * 2.0f, 0.0f });

	sprtEnt1.getComponent<clv::ecs::_2D::TransformComponent>()->setPosition(clv::math::Vector2f(cos(rotDelta) * radius * 5.0f, 0.0f));
	sprtEnt1.getComponent<clv::ecs::_2D::TransformComponent>()->setRotation(rotDelta);

	rotDelta += rotSpeed * deltaTime;

	if(clv::input::isKeyPressed(clv::Key::Escape)){
		clv::Application::get().stop();
	}

	//Audio testing
	if(clv::input::isKeyPressed(clv::Key::P)){
		sound.getComponent<clv::ecs::aud::AudioComponent>()->pause();
	}

	if(clv::input::isKeyPressed(clv::Key::O)){
		sound.getComponent<clv::ecs::aud::AudioComponent>()->stop();
	}

	if(clv::input::isKeyPressed(clv::Key::L)){
		sound.getComponent<clv::ecs::aud::AudioComponent>()->play(clv::ecs::aud::PlaybackMode::once);
	}

	if(clv::input::isKeyPressed(clv::Key::K)){
		sound.getComponent<clv::ecs::aud::AudioComponent>()->play(clv::ecs::aud::PlaybackMode::repeat);
	}

	if(clv::input::isKeyPressed(clv::Key::Semicolon)){
		sound.getComponent<clv::ecs::aud::AudioComponent>()->resume();
	}

	//Print FPS
	secondsPassed += deltaTime;
	clv::uint64 avgFPS = countedFrames / (secondsPassed); //This needs to be total seconds passed not time between frames
	if(avgFPS > 20000){
		avgFPS = 0;
	}

	std::ostringstream outStream;
	outStream << "Average FPS: " << avgFPS;
	fpsEnt.getComponent<clv::ecs::ui::TextComponent>()->setText(outStream.str());

	++countedFrames;
}

//bool TestLayer::onMouseMoved(clv::MouseMovedEvent e){
//	if(firstMouse){
//		lastMouseX = e.getX();
//		lastMouseY = e.getY();
//		firstMouse = false;
//	}
//
//	float xoffset = static_cast<float>(e.getX() - lastMouseX);
//	float yoffset = static_cast<float>(lastMouseY - e.getY());
//	lastMouseX = e.getX();
//	lastMouseY = e.getY();
//
//	float sensitivity = 0.05f;
//	xoffset *= sensitivity;
//	yoffset *= sensitivity;
//
//	yaw += xoffset;
//	pitch += yoffset;
//
//	if(pitch > 89.0f){
//		pitch = 89.0f;
//	}
//	if(pitch < -89.0f){
//		pitch = -89.0f;
//	}
//
//	return true;
//}