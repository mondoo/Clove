set(ExamplesFolder "Garlic/Bulb/Examples")

add_subdirectory(RotatingCube)
set_target_properties(RotatingCube PROPERTIES FOLDER ${ExamplesFolder})

add_subdirectory(RenderingAudio)
set_target_properties(RenderingAudio PROPERTIES FOLDER ${ExamplesFolder})