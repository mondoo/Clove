#include "TestLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/ECS/Manager.hpp"
#include "Clove/ECS/3D/Components/RenderableComponent.hpp"
#include "Clove/ECS/2D/Components/RenderableComponent.hpp"
#include "Clove/ECS/2D/Components/TransformComponent.hpp"
#include "Clove/ECS/3D/Components/TransformComponent.hpp"
#include "Clove/ECS/3D/Components/CameraComponent.hpp"
#include "Clove/ECS/3D/Components/LightComponent.hpp"
#include "Clove/ECS/Audio/Components/AudioComponent.hpp"
#include "Clove/Maths/Vector.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/Utils/DeltaTime.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/RenderTarget.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Graphics/Material.hpp"

TestLayer::TestLayer()
	: Layer("Example Layer"){
}

void TestLayer::onAttach(){
	ent1 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();
	ent2 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();
	ent3 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();

	//rtEnt = clv::Application::get().getManager().createEntity<clv::ecs::d2::RenderableComponent, clv::ecs::d2::TransformComponent>();

	sprtEnt1 = clv::Application::get().getManager().createEntity<clv::ecs::d2::RenderableComponent, clv::ecs::d2::TransformComponent>();
	sprtEnt2 = clv::Application::get().getManager().createEntity<clv::ecs::d2::RenderableComponent, clv::ecs::d2::TransformComponent>();

	lght1 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::LightComponent, clv::ecs::d3::TransformComponent>();
	lght2 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::LightComponent, clv::ecs::d3::TransformComponent>();

	cam = clv::Application::get().getManager().createEntity<clv::ecs::d3::CameraComponent, clv::ecs::d3::TransformComponent>();

	sound = clv::Application::get().getManager().createEntity<clv::ecs::aud::AudioComponent>();

	auto cubeMaterial = std::make_shared<clv::gfx::Material>(clv::gfx::ShaderStyle::Lit);
	cubeMaterial->setAlbedoTexture("res/Textures/container2.png");
	cubeMaterial->setSpecularTexture("res/Textures/container2_specular.png");

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		ent1.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh(mesh);
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		ent2.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh(mesh);
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		ent3.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh(mesh);
	}

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ 0.0f, 0.0f, 0.0f });
	ent2.getComponent<clv::ecs::d3::TransformComponent>()->setLocalPosition({ 0.0f, 0.0f, 3.0f });
	ent3.getComponent<clv::ecs::d3::TransformComponent>()->setLocalPosition({ 0.0f, 3.0f, 0.0f });

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->addChild(ent2.getComponent<clv::ecs::d3::TransformComponent>());
	ent2.getComponent<clv::ecs::d3::TransformComponent>()->addChild(ent3.getComponent<clv::ecs::d3::TransformComponent>());

	//clv::Application::get().getManager().destroyEntity(ent2.getID());

	/*auto rtTexture = clv::gfx::BindableFactory::createTexture(clv::Application::get().getWindow().getWidth(), clv::Application::get().getWindow().getHeight(), clv::gfx::TextureUsage::RenderTarget, clv::gfx::TBP_Albedo);
	auto sprite = std::make_shared<clv::gfx::Sprite>(rtTexture);
	rtEnt.getComponent<clv::ecs::d2::RenderableComponent>()->setSprite(sprite);
	rtEnt.getComponent<clv::ecs::d2::TransformComponent>()->setScale(clv::math::Vector2f(clv::Application::get().getWindow().getWidth() / 2, clv::Application::get().getWindow().getHeight() / 2));

	auto renderTarget = clv::gfx::RenderTarget::createRenderTarget(*rtTexture);
	clv::gfx::Renderer::setRenderTarget(renderTarget);*/

	{
		auto sprite = std::make_shared<clv::gfx::Sprite>("res/Textures/Zombie-32x32.png");
		sprtEnt1.getComponent<clv::ecs::d2::RenderableComponent>()->setSprite(sprite);
		sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->setScale(clv::math::Vector2f(20.0f, 20.0f));
	}

	{
		auto sprite = std::make_shared<clv::gfx::Sprite>("res/Textures/Zombie-32x32.png");
		sprtEnt2.getComponent<clv::ecs::d2::RenderableComponent>()->setSprite(sprite);
		sprtEnt2.getComponent<clv::ecs::d2::TransformComponent>()->setLocalPosition(clv::math::Vector2f(0.0f, 2.0f));
		sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->setScale(clv::math::Vector2f(20.0f, 20.0f));
	}

	sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->addChild(sprtEnt2.getComponent<clv::ecs::d2::TransformComponent>());

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		lght1.getComponent<clv::ecs::d3::TransformComponent>()->setScale({ 0.25f, 0.25f, 0.25f });
		lght1.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh(mesh);
	}

	{
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());
		lght2.getComponent<clv::ecs::d3::TransformComponent>()->setScale({ 0.25f, 0.25f, 0.25f });
		lght2.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh(mesh);
	}

	{
		auto soundBuffer = clv::aud::Sound("res/Audio/Test.wav");
		sound.getComponent<clv::ecs::aud::AudioComponent>()->setSound(soundBuffer);
		sound.getComponent<clv::ecs::aud::AudioComponent>()->play();
	}
}

void TestLayer::onDetach(){

}

void TestLayer::onUpdate(clv::utl::DeltaTime deltaTime){
	const float camSpeed = 15.0f;
	const float camDelta = camSpeed * deltaTime;
	const float rotSpeed = 1.5f;
	//const float deltaTime = deltaTime;

	float s = 6 * deltaTime;

	clv::math::Vector3f cameraPosition = cam.getComponent<clv::ecs::d3::TransformComponent>()->getPosition();

	//cam->updateFront(pitch, yaw); //TODO: proper first person implementation

	const clv::math::Vector3f front = cam.getComponent<clv::ecs::d3::CameraComponent>()->getFront();
	if(clv::input::isKeyPressed(clv::Key::W)){
		cameraPosition += front * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::S)){
		cameraPosition -= front * camDelta;
	}

	const clv::math::Vector3f up = cam.getComponent<clv::ecs::d3::CameraComponent>()->getUp();
	if(clv::input::isKeyPressed(clv::Key::Space)){
		cameraPosition += up * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::C)){
		cameraPosition -= up * camDelta;
	}

	const clv::math::Vector3f right = cam.getComponent<clv::ecs::d3::CameraComponent>()->getRight();
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

	cam.getComponent<clv::ecs::d3::TransformComponent>()->setPosition(cameraPosition);
	cam.getComponent<clv::ecs::d3::CameraComponent>()->updateFront(0.0f, yaw);

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->setRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 0.0f, 1.0f, 0.0f }));
	ent2.getComponent<clv::ecs::d3::TransformComponent>()->setLocalRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 0.0f, 0.0f, 1.0f }));
	ent3.getComponent<clv::ecs::d3::TransformComponent>()->setLocalRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 1.0f, 0.0f, 0.0f }));

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ cos(rotDelta) * radius, 0.0f, 0.0f });

	lght1.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ cos(rotDelta * 1.5f) * radius * 2.0f, 0.0f, sin(rotDelta * 1.5f) * radius * 2.0f });
	lght2.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ cos(rotDelta) * radius * 2.0f, sin(rotDelta) * radius * 2.0f, 0.0f });

	sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->setPosition(clv::math::Vector2f(cos(rotDelta) * radius * 5.0f, 0.0f));
	sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->setRotation(rotDelta);

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