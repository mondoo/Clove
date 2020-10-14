if(GAR_BUILD_TESTS)
    set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.")
    add_subdirectory(googletest)
endif()