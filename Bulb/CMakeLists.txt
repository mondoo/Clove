#Bulb dependencies
set(EditorFolder "Garlic/Bulb")

add_subdirectory(Bulb.Core)
set_target_properties(Bulb.Core PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.UI)
set_target_properties(Bulb.UI PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.Windows)
set_target_properties(Bulb.Windows PROPERTIES FOLDER ${EditorFolder}) 