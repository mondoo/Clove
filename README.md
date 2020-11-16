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

Currently, **Clove** supports the following platforms:

|**Platform**|**API**|
|-|-|
|Windows|Vulkan|
|Linux|Vulkan|
|MacOS(WIP)|Metal(WIP)|