#Clove dependencies
set(EngineFolder "Clove")
set(DependencyFolder "Clove/Dependencies")

#Vendor defaults
set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")
set(PA_BUILD_SHARED OFF CACHE BOOL "Build shared/dynamic library")
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos")
set(BUILD_CLSOCKET OFF CACHE BOOL "Set when you want to build apps with enet TCP networking support")
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Set to build original Bullet CPU demos")
set(BUILD_EGL OFF CACHE BOOL "OpenGL/EGL")
set(BUILD_ENET OFF CACHE BOOL "Set when you want to build apps with enet UDP networking support")
set(BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build extras")
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Set when you want to build Bullet 3 OpenGL3+ demos")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Build unit tests")

#-dxerr
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory(vendor/dxerr)
	set_target_properties(dxerr PROPERTIES FOLDER ${DependencyFolder})
endif()

#-Glad
add_subdirectory(vendor/Glad)
set_target_properties(Glad PROPERTIES FOLDER ${DependencyFolder})

#-glm
add_subdirectory(vendor/glm)
set_target_properties(glm_static PROPERTIES FOLDER ${DependencyFolder}/glm)
set_target_properties(uninstall PROPERTIES FOLDER ${DependencyFolder}/glm)

#-spdlog
add_subdirectory(vendor/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER ${DependencyFolder})

#-stb
add_subdirectory(vendor/stb)
set_target_properties(stb PROPERTIES FOLDER ${DependencyFolder})

#-portaudio
add_subdirectory(vendor/portaudio)
set_target_properties(portaudio_static PROPERTIES FOLDER ${DependencyFolder})

#-libsndfile
add_subdirectory(vendor/libsndfile)
set_target_properties(sndfile PROPERTIES FOLDER ${DependencyFolder})

#-freetype2
add_subdirectory(vendor/freetype2)
set_target_properties(freetype PROPERTIES FOLDER ${DependencyFolder})

#-bullet3
add_subdirectory(vendor/bullet3)
set_target_properties(Bullet2FileLoader PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(Bullet3Collision PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(Bullet3Common PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(Bullet3Dynamics PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(Bullet3Geometry PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(Bullet3OpenCL_clew PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(BulletCollision PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(BulletDynamics PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(BulletInverseDynamics PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(BulletSoftBody PROPERTIES FOLDER ${DependencyFolder}/bullet3)
set_target_properties(LinearMath PROPERTIES FOLDER ${DependencyFolder}/bullet3)

#Clove
add_subdirectory(src)
set_target_properties(Clove PROPERTIES FOLDER ${EngineFolder})