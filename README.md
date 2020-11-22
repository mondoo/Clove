# Garlic

The goal of Garlic is to create an extremely modular engine where it's libraries can be used independently.

Please note that the engine is still in very early development and so the API will fluctuate wildly and without warning!

The Garlic game engine is comprised of the following components:

|Component|Language|Description|
|-|-|-|
|Root|C++|A collection of core libraries for the entire engine (Definitions, Events, Memory etc.)|
|Clove|C++|A collection of libraries to provide various system abstractions (Graphics, Platform, Audio etc.)|
|Bulb|C++|A collection of libraries for developing games (AI, ECS, etc.)|
|Stem|C++|The library that brings the engine together. Built with Root, Clove and Bulb|
|Scape|C#|Garlic's Editor, built on top of Stem|

Note: Scape is only currently supported on Windows. This could change if CMake has better support for C# on Linux and MacOS.  

Currently, **Clove** supports the following platforms:

|**Platform**|**API**|
|-|-|
|Windows|Vulkan|
|Linux|Vulkan|
|MacOS(WIP)|Metal(WIP)|

## Building

Garlic is built using CMake.  
Inside the project directory, use the following commands to generate and build the project:
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Linking

If using Garlic as a `sub_directory` inside a CMakeLists then OpenAL will need to be placed into the same directory as your exe.  
This can be achieved by putting the following snippet inside the CMakeLists that builds your executable:
```
set_target_properties(OpenAL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
```