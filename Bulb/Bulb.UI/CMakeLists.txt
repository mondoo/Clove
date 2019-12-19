project(Bulb.UI)

set(
    Files
        ViewModel.cs
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Files})

add_library(
    ${PROJECT_NAME} SHARED
        ${Files}
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES "System")