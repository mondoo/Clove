#pragma once

//Clove ImGui config options

//Even though we statically link we need ImGui to export so that applications including our dll can link ImGui
#define IMGUI_API __declspec(dllexport)