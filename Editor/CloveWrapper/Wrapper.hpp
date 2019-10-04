#pragma once

//Reg C++/CLR (No works :( )
namespace CloveWrapper{
	public ref class Wrapper {
	public:
		void OpenClove();
	};
}

//Reg C++ (Works)
//extern "C"{
//	__declspec(dllexport) void OpenClove();
//}