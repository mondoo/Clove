#Bulb dependencies
set(EditorFolder "Garlic/Bulb")

add_subdirectory(Bulb.Core)
set_target_properties(Bulb.Core PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.CoreUI)
set_target_properties(Bulb.CoreUI PROPERTIES FOLDER ${EditorFolder}) 

add_subdirectory(Bulb.Desktop)
set_target_properties(Bulb.Desktop PROPERTIES FOLDER ${EditorFolder})

add_subdirectory(Bulb.Windows)
set_target_properties(Bulb.Windows PROPERTIES FOLDER ${EditorFolder})

#set_property(TARGET MyApplication
#    PROPERTY VS_PACKAGE_REFERENCES "BouncyCastle_1.8.5"
#)