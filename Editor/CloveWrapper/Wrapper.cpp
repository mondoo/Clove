#include "Wrapper.hpp"
//#include "clvpch.hpp"//TODO: See note in example layer
//#include <Clove.hpp>
//
//class Editor : public clv::Application{
//public:
//	Editor(){
//		//pushLayer(std::make_shared<TestLayer>());
//		//pushLayer(std::make_shared<GameLayer>());
//	}
//	~Editor(){
//
//	}
//
//private:
//
//};
//
//clv::Application* clv::createApplication(){
//	return new Editor;
//}

void Set(int val){
	globaldata = val;
}

int Get(){
	return globaldata;
}
