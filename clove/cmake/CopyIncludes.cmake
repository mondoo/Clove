function(copy_target_includes_to_out_dir_for_target target for_target)
    get_target_property(includes ${target} INTERFACE_INCLUDE_DIRECTORIES)

	add_custom_command(
		TARGET ${for_target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${includes} $<TARGET_FILE_DIR:${for_target}>/include
	)
endfunction()

function(copy_target_includes_to_out_dir target)
    copy_target_includes_to_out_dir_for_target(${target} ${target})
endfunction()