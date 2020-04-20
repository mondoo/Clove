set(ApplicationFolder "Garlic/Stem")

project(Stem)

add_library(${PROJECT_NAME} STATIC main.cpp)
set_target_properties(Stem PROPERTIES FOLDER ${ApplicationFolder})

target_link_libraries(Stem PUBLIC Bulb)