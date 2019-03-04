#include "clvpch.h"
#include <Clove.h>

class SandBox : public clv::Application{
public:
	SandBox(){

	}
	~SandBox(){

	}

private:

};

clv::Application* clv::createApplication(){
	return new SandBox;
}