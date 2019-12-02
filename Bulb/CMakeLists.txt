#Bulb dependencies
set(EditorFolder "Garlic/Bulb")

#-UI
add_subdirectory(Bulb.UI)
set_target_properties(Bulb.UI PROPERTIES FOLDER ${EditorFolder}) 

#-engine
add_subdirectory(Bulb.Engine.Clove)
set_target_properties(Bulb.Engine.Clove PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.Engine)
set_target_properties(Bulb.Engine PROPERTIES FOLDER ${EditorFolder}) 

#-windows
add_subdirectory(Bulb.Desktop.Windows)
set_target_properties(Bulb.Desktop.Windows PROPERTIES FOLDER ${EditorFolder}) 