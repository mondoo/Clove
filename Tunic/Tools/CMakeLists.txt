set(ToolsFolder "Garlic/Tunic/Tools")

#ResourceEmbedder
add_subdirectory(ResourceEmbedder)
set_target_properties(ResourceEmbedder PROPERTIES FOLDER ${ToolsFolder})
