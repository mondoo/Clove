#Bulb dependencies
set(EditorFolder "Garlic/Bulb")

add_subdirectory(CloveWrapper)
set_target_properties(CloveWrapper PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.Core)
set_target_properties(Bulb.Core PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.Windows)
set_target_properties(Bulb.Windows PROPERTIES FOLDER ${EditorFolder}) 