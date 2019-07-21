#include <Clove.hpp>

#include "ExampleLayer.hpp"
#include "GameLayer.hpp"

class SandBox : public clv::Application{
public:
	SandBox(){
		//pushLayer(std::make_shared<ExampleLayer>());
		pushLayer(std::make_shared<GameLayer>());
	}
	~SandBox(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new SandBox;
}