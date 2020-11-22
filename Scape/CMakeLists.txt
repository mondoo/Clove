#Create a new project to set the language of the editor. This prevents issues linux / macos by setting it on the main project.
project(Garlic-Editor LANGUAGES C CXX CSharp)

add_subdirectory(Editor)
add_subdirectory(Wrapper)
add_subdirectory(Membrane)