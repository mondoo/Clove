# Garlic

The goal of Garlic is to create an extremely modular engine where it's libraries can be used independently.

Please note that the engine is still in very early development and so the API will fluctuate wildly and without warning!

The Garlic game engine is comprised of the following components:

|Component|Language|Description|
|-|-|-|
|Clove|C++|Clove is the engine itself. This is where all the platform / graphics / rendering / systems etc. are|
|Bulb|C#|Bulb is the editor. It contains all of the view logic|

Note: Bulb is only currently supported on Windows.

Currently, **Clove** supports the following platforms:

|**Platform**|**API**|
|-|-|
|Windows|Vulkan|
|Linux|Vulkan|
|MacOS|Metal|

## Building

Garlic is built using CMake.  
Inside the project directory, use the following commands to generate and build the project:
```
mkdir build
cd build
cmake ..
cmake --build .
```