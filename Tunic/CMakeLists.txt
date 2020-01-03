set(FrameworkFolder "Garlic/Tunic")

add_subdirectory(src)
set_target_properties(Tunic PROPERTIES FOLDER ${FrameworkFolder})

target_include_directories(
	Tunic
	#Tunic
	PUBLIC
		include
)

target_link_libraries(
	Tunic
    #Cross platform
    PUBLIC
        Clove
)