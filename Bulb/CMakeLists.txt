set(FrameworkFolder "Garlic/Bulb")

#Dependencies
add_subdirectory(vendor)

#Tools
add_subdirectory(Tools)

#Bulb
add_subdirectory(src)
set_target_properties(Bulb PROPERTIES FOLDER ${FrameworkFolder})

#Includes
target_include_directories(
	Bulb

	#Bulb
	PUBLIC
		include

	PRIVATE
		vendor/freetype2/include
)

#Libraries
target_link_libraries(
	Bulb

	#Cross Platform
    PUBLIC
		Clove

	PRIVATE
		freetype
		assimp
		sndfile
		stb
)