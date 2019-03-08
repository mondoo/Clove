#include "clvpch.h"
#include <Clove.h>

#include "Clove/Events/Event.h"

class ExampleLayer : public clv::Layer{
	//FUNCTIONS
public:
	ExampleLayer() : Layer("Example"){
	}

	virtual void onUpdate() override{
		
	}

	virtual void onEvent(clv::Event& e) override{
		
	}
};

class SandBox : public clv::Application{
public:
	SandBox(){
		pushLayer(new ExampleLayer());
		pushOverlay(new clv::ImGuiLayer());
	}
	~SandBox(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new SandBox;
}