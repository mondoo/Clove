set(ToolsFolder "Garlic/Bulb/Tools")

#ResourceEmbedder
add_subdirectory(ResourceEmbedder)
set_target_properties(ResourceEmbedder PROPERTIES FOLDER ${ToolsFolder})
