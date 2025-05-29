function(add_defines_for_test target_name)
	if(testing_build)
		target_compile_definitions(${target_name}
			PUBLIC
				${target_name}_TEST=1
		)
	endif()
endfunction()


function(add_defines_for_platform target_name)
	set(target_platform 0)
	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
		set(target_platform 1)
	elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
		set(target_platform 2)
	endif()

	target_compile_definitions(${target_name}
		PUBLIC	
			${target_name}_PLATFORM=${target_platform}
	)
endfunction()	


function(add_defines_for_compiler target_name)	
	set(target_compiler 0)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(target_compiler 1)
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(target_compiler 2)
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(target_compiler 3)
	endif()

	target_compile_definitions(${target_name}
		PUBLIC	
		${target_name}_COMPILER=${target_compiler}
	)
endfunction()


function(add_defines_for_building target_name)
	target_compile_definitions(${target_name}
		PRIVATE
			${target_name}_INSIDE=1
	)
endfunction()


function(add_defines_for_build_config ${target_name})
	set(build_config 0)
	
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(build_config 1)
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		set(build_config 2)
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
		set(build_config 3)
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
		set(build_config 4)
	endif()

	target_compile_definitions(${target_name}
		PRIVATE
			${target_name}_BUILD_TYPE=${build_config}

	)

endfunction()
