set(ToolsFolder "Garlic/Stem/Tools")

#ResourceEmbedder
add_subdirectory(ResourceEmbedder)
set_target_properties(ResourceEmbedder PROPERTIES FOLDER ${ToolsFolder})
