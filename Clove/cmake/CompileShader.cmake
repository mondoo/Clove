function(compileShader input output shaderStage)
	add_custom_command(
		OUTPUT ${output}
		COMMAND glslc -fshader-stage=${shaderStage} ${input} -o ${output}
		DEPENDS ${input}
	)
endfunction()