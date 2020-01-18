set(FrameworkFolder "Garlic/Tunic")

#Dependencies
add_subdirectory(vendor)

#Tunic
add_subdirectory(src)
set_target_properties(Tunic PROPERTIES FOLDER ${FrameworkFolder})

target_include_directories(
	Tunic
	#Tunic
	PUBLIC
		include
	#Libraries
	PRIVATE
		vendor/bullet3/src
)

target_link_libraries(
	Tunic
    #Cross platform
    PUBLIC
		Clove
	PRIVATE
		#Order of these are important
		BulletDynamics
		BulletCollision
		LinearMath
)