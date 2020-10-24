set(EditorFolder "Garlic/Scape")

#add_subdirectory(Scape.Editor)
add_subdirectory(Membrane)

#set_target_properties(Scape.Editor PROPERTIES FOLDER ${EditorFolder})
set_target_properties(Scape.Membrane PROPERTIES FOLDER ${EditorFolder})