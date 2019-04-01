#pragma once

//Clove ImGui config options

//When clv is compiled as a DLL we need to export ImGUI
#if CLV_DYNAMIC
	#define IMGUI_API __declspec(dllexport)
#endif