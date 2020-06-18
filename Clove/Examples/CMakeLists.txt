set(ExamplesFolder "Garlic/Clove/Examples")

add_subdirectory(HelloTriangle)
set_target_properties(HelloTriangle PROPERTIES FOLDER ${ExamplesFolder})

add_subdirectory(PlayingAudio)
set_target_properties(PlayingAudio PROPERTIES FOLDER ${ExamplesFolder})