set(ExamplesFolder "Garlic/Bulb/Examples")

add_subdirectory(RotatingCube)
set_target_properties(RotatingCube PROPERTIES FOLDER ${ExamplesFolder})

add_subdirectory(PlayingAudio)
set_target_properties(PlayingAudio PROPERTIES FOLDER ${ExamplesFolder})

add_subdirectory(StreamingAudio)
set_target_properties(StreamingAudio PROPERTIES FOLDER ${ExamplesFolder})