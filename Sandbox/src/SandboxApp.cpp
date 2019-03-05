#include "clvpch.h"
#include <Clove.h>

#include "Clove/Events/Event.h"

class ExampleLayer : public clv::Layer{
	//FUNCTIONS
public:
	ExampleLayer() : Layer("Example"){
	}

	virtual void onUpdate() override{
		CLV_INFO("ExampleLayer::onUpdate");
	}

	virtual void onEvent(clv::Event& e) override{
		CLV_TRACE("{0}", e);
	}
};

class SandBox : public clv::Application{
public:
	SandBox(){
		pushLayer(new ExampleLayer());
	}
	~SandBox(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new SandBox;
}