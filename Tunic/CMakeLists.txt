set(FrameworkFolder "Garlic/Tunic")

#Dependencies
add_subdirectory(vendor)

#Tunic
add_subdirectory(src)
set_target_properties(Tunic PROPERTIES FOLDER ${FrameworkFolder})

#Includes
target_include_directories(
	Tunic

	#Tunic
	PUBLIC
		include

	#Libraries
	INTERFACE
		#TODO: Get rid of the lbsndfile paths below (see Sound.hpp)
		vendor/libsndfile/src
		${CMAKE_CURRENT_BINARY_DIR}/vendor/libsndfile/src

	PRIVATE
		vendor/freetype2/include
		vendor/bullet3/src
)

#Libraries
target_link_libraries(
	Tunic

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
)

#Examples
if(GAR_BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()