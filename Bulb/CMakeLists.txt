set(ApplicationFolder "Garlic/Bulb")

project(Bulb)

add_library(${PROJECT_NAME} STATIC main.cpp)
set_target_properties(Bulb PROPERTIES FOLDER ${ApplicationFolder})

target_link_libraries(Bulb PUBLIC Tunic)