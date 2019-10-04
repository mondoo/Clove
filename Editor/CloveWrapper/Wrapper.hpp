#pragma once

int globaldata;
extern "C" __declspec(dllexport) void Set(int val);
extern "C" __declspec(dllexport) int  Get();