set(DependencyFolder "Garlic/Tunic/Dependencies")

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