set(ExamplesFolder "Garlic/Bulb/Examples")

add_subdirectory(RotatingCube)
set_target_properties(RotatingCube PROPERTIES FOLDER ${ExamplesFolder})

add_subdirectory(SkeletalAnimation)
set_target_properties(SkeletalAnimation PROPERTIES FOLDER ${ExamplesFolder})