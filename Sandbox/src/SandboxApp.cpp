#include "clvpch.h"
#include <Clove.h>

#include "Clove/Events/Event.h"

class ExampleLayer : public clv::Layer{
	//FUNCTIONS
public:
	ExampleLayer() 
		: Layer("Example"){
	}

	virtual void onAttach() override{
		CLV_TRACE("On attached called");
	}

	virtual void onDetach() override{
		CLV_TRACE("On detached called");
	}

	virtual void onUpdate() override{
		
	}

	virtual void onImGuiRender() override{
		
	}

	virtual void onEvent(clv::Event& e) override{
		
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