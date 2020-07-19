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
		vendor/bullet3/src
)

#Libraries
target_link_libraries(
	Bulb

	#Cross Platform
    PUBLIC
		Clove

	PRIVATE
		freetype
		#Order of these are important
		BulletDynamics
		BulletCollision
		LinearMath
		#
		assimp
		sndfile
		stb
)

#Examples
if(GAR_BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()