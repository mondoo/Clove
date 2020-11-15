set(DependencyFolder "Garlic/Bulb/Dependencies")

#libsndfile
add_subdirectory(libsndfile)
set_target_properties(sndfile PROPERTIES FOLDER ${DependencyFolder})
