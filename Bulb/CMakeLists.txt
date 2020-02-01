#Bulb dependencies
set(EditorFolder "Garlic/Bulb")

add_subdirectory(Bulb.Core)
set_target_properties(Bulb.Core PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.CoreUI)
set_target_properties(Bulb.CoreUI PROPERTIES FOLDER ${EditorFolder}) 

include_external_msproject(Bulb.Desktop ${CMAKE_CURRENT_SOURCE_DIR}/Bulb.Desktop/Bulb.Desktop.csproj)
set_target_properties(Bulb.Desktop PROPERTIES FOLDER ${EditorFolder})

add_subdirectory(Bulb.Windows)
set_target_properties(Bulb.Windows PROPERTIES FOLDER ${EditorFolder})