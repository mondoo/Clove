set(EditorFolder "Garlic/Scape")

include_external_msproject(Scape.Editor "${CMAKE_CURRENT_SOURCE_DIR}/Scape.Editor/Scape.Editor.csproj")
add_subdirectory(Scape.Membrane)

set_target_properties(Scape.Editor PROPERTIES FOLDER ${EditorFolder})
set_target_properties(Scape.Membrane PROPERTIES FOLDER ${EditorFolder})