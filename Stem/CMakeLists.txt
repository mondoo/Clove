set(ApplicationFolder "Garlic/Stem")

#Stem
add_subdirectory(src)

set_target_properties(Stem PROPERTIES FOLDER ${ApplicationFolder})

#Includes
target_include_directories(
	Stem
	PUBLIC
		include
)

#Libraries
target_link_libraries(
	Stem 
	PUBLIC 
		Bulb
)