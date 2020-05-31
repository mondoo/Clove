set(DependencyFolder "Garlic/Bulb/Dependencies")

#Vendor defaults
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos")
set(BUILD_CLSOCKET OFF CACHE BOOL "Set when you want to build apps with enet TCP networking support")
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Set to build original Bullet CPU demos")
set(BUILD_EGL OFF CACHE BOOL "OpenGL/EGL")
set(BUILD_ENET OFF CACHE BOOL "Set when you want to build apps with enet UDP networking support")
set(BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build extras")
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Set when you want to build Bullet 3 OpenGL3+ demos")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Build unit tests")
set(USE_GLUT OFF CACHE BOOL "Use GLUT")
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "Use graphical benchmark")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suit for assimp is built in addition to the library")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplimentary tools for Assimp are built in addition to the library")
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "If the Assimp view tool is built. (requires DirectX)")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "default value of all ASSIMP_BUILD_XXX_EXPORTER values")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "default value of all ASSIMP_BUILD_XXX_IMPORTER values")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable this if you want to use assimp as a submodule")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Install MSVC debug files")

#freetype2
add_subdirectory(freetype2)
set_target_properties(freetype PROPERTIES FOLDER ${DependencyFolder})

#bullet3
add_subdirectory(bullet3)
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

#assimp
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
add_subdirectory(assimp)
set_target_properties(assimp PROPERTIES FOLDER ${DependencyFolder}/assimp)
set_target_properties(IrrXML PROPERTIES FOLDER ${DependencyFolder}/assimp)
if(MSVC)
    set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER ${DependencyFolder}/assimp)
endif()

#portaudio
add_subdirectory(portaudio)
set_target_properties(portaudio_static PROPERTIES FOLDER ${DependencyFolder}/portaudio)
set_target_properties(uninstall PROPERTIES FOLDER ${DependencyFolder}/portaudio)

#libsndfile
add_subdirectory(libsndfile)
set_target_properties(sndfile PROPERTIES FOLDER ${DependencyFolder})