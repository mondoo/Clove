# Garlic
![build](https://github.com/AGarlicMonkey/Garlic/workflows/build/badge.svg)

The Garlic game engine is comprised of the following components:

|Component|Language|Description|
|-|-|-|
|Clove|C++|The abstraction layer (Graphics, Platform, Audio etc.)|
|Tunic|C++|Built around Clove, implements higher level systems used in games (AI, ECS, Physics, etc.)|
|Bulb(WIP)|C++|A prebuilt application layer built ontop of Tunic. Provides a starting point for games|
|Stem(WIP)|C#|A WYSIWYG Editor built on top of Bulb|

Currently, **Clove** supports the following platforms:

|**Platform**|**API**|
|-|-|
|Windows|Direct3D 11, OpenGL 4.6|
|Linux|OpenGL 4.6|
|MacOS(WIP)|Metal 1(WIP)|

[For more info: visit the wiki](https://github.com/AGarlicMonkey/Clove/wiki)
