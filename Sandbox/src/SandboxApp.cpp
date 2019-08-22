#include "clvpch.hpp"//TODO: See note in example layer
#include <Clove.hpp>

#include "TestLayer.hpp"
#include "GameLayer.hpp"

class SandBox : public clv::Application{
public:
	SandBox(){
		pushLayer(std::make_shared<GameLayer>());
	}
	~SandBox(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new SandBox;
}