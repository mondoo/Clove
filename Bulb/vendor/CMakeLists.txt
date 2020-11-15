set(DependencyFolder "Garlic/Bulb/Dependencies")

#freetype2
add_subdirectory(freetype2)
set_target_properties(freetype PROPERTIES FOLDER ${DependencyFolder})

#assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suit for assimp is built in addition to the library")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplimentary tools for Assimp are built in addition to the library")
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "If the Assimp view tool is built. (requires DirectX)")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "default value of all ASSIMP_BUILD_XXX_EXPORTER values")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "default value of all ASSIMP_BUILD_XXX_IMPORTER values")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable this if you want to use assimp as a submodule")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Install MSVC debug files")
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "Build your own zlib")
add_subdirectory(assimp)
set_target_properties(assimp PROPERTIES FOLDER ${DependencyFolder}/assimp)
set_target_properties(IrrXML PROPERTIES FOLDER ${DependencyFolder}/assimp)
if(MSVC)
    set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER ${DependencyFolder}/assimp)
endif()

#libsndfile
add_subdirectory(libsndfile)
set_target_properties(sndfile PROPERTIES FOLDER ${DependencyFolder})

#stb
add_subdirectory(stb)
set_target_properties(stb PROPERTIES FOLDER ${DependencyFolder})