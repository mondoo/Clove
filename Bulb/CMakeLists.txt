#Bulb dependencies
set(EditorFolder "Bulb")

#-core
add_subdirectory(Bulb.Core)
set_target_properties(Bulb.Core PROPERTIES FOLDER ${EditorFolder}) 

#-UI
add_subdirectory(Bulb.UI)
set_target_properties(Bulb.UI PROPERTIES FOLDER ${EditorFolder}) 

#-engine
add_subdirectory(Bulb.Engine)
set_target_properties(Bulb.Engine PROPERTIES FOLDER ${EditorFolder}) 

#-windows
add_subdirectory(Bulb.Desktop.Windows)
set_target_properties(Bulb.Desktop.Windows PROPERTIES FOLDER ${EditorFolder}) 