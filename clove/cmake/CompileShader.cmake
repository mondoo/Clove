#include Has no effect on the output. It just makes sure the dependecies are correct
function(compileShader input output shaderStage include)
	add_custom_command(
		OUTPUT ${output}
		COMMAND glslc -fshader-stage=${shaderStage} ${input} -o ${output}
		DEPENDS ${input} ${include}
	)
endfunction()