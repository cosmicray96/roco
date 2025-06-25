
function(get_public_sources target_name public_sources)
	set(path_to_target "${CMAKE_SOURCE_DIR}/projects/${target_name}")
	
	set(target_sources_public "")
	if("${language}" STREQUAL "CPP")
	file(GLOB_RECURSE target_sources_public 
		"${path_to_target}/inc/${target_name}/*.hpp"
		"${path_to_target}/inc/${target_name}/*.h"
	)
	elseif("${language}" STREQUAL "C")
	file(GLOB_RECURSE target_sources_public 
		"${path_to_target}/inc/${target_name}/*.h"
	)
	endif()

	set(public_sources "${target_sources_public}" PARENT_SCOPE)
endfunction()


function(get_private_sources target_name private_sources)
	set(path_to_target "${CMAKE_SOURCE_DIR}/projects/${target_name}")
	

	set(target_sources_private "")
	if("${language}" STREQUAL "CPP")
	file(GLOB_RECURSE target_sources_private 
		"${path_to_target}/src/${target_name}/*.hpp"
		"${path_to_target}/src/${target_name}/*.cpp"
		"${path_to_target}/src/${target_name}/*.h"
		"${path_to_target}/src/${target_name}/*.c"
	)
	elseif("${language}" STREQUAL "C")
	file(GLOB_RECURSE target_sources_private 
		"${path_to_target}/src/${target_name}/*.h"
		"${path_to_target}/src/${target_name}/*.c"
	)
	endif()

	set(private_sources "${target_sources_private}" PARENT_SCOPE)
endfunction()


function(target_common target_name)
	set_target_properties(${target_name}
		PROPERTIES
			DEFINE_SYMBOL ""
	)
	
	# This ensures symbols are hidden by default on GCC/Clang
	set_target_properties(${target_name} PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN YES
	)

	add_defines_for_platform(${target_name})	
	add_defines_for_compiler(${target_name})	
	add_defines_for_building(${target_name})	
	add_defines_for_build_config(${target_name})	

endfunction()


function(add_static_target target_name)

	set(public_sources "")
	set(private_sources "")
	get_public_sources(${target_name} public_sources)
	get_private_sources(${target_name} private_sources)

	add_library(${target_name} STATIC
		${private_sources}
	)
	target_sources(${target_name} 
		PUBLIC
			${public_sources}
	)	

	target_common(${target_name})

	set(path_to_target "${CMAKE_SOURCE_DIR}/projects/${target_name}")
	target_include_directories(${target_name}
		PUBLIC
			"${path_to_target}/inc/"
		PRIVATE
			"${path_to_target}/src/"
	)

message("Target Added: ${target_name} (Static Library)")

endfunction()


function(add_executable_target target_name)
	
	set(private_sources "")
	get_private_sources(${target_name} private_sources)
	add_executable(${target_name} 
		${private_sources}
	)

	target_common(${target_name})

	set(path_to_target "${CMAKE_SOURCE_DIR}/projects/${target_name}")
	target_include_directories(${target_name}
		PUBLIC
			"${path_to_target}/inc/"
		PRIVATE
			"${path_to_target}/src/"
	)

	set_target_properties(${target_name}
		PROPERTIES
			INSTALL_RPATH "$ORIGIN"
	)

message("Target Added: ${target_name} (Executable)")

endfunction()



function(add_dynamic_target target_name)
	
	set(public_sources "")
	set(private_sources "")
	get_public_sources(${target_name} public_sources)
	get_private_sources(${target_name} private_sources)
	add_library(${target_name} SHARED
		${private_sources})
	target_sources(${target_name} 
		PUBLIC
			"${public_sources}"
	)	

	add_library(${target_name} SHARED)
	target_common(${target_name})

	set(path_to_target "${CMAKE_SOURCE_DIR}/projects/${target_name}")
	target_include_directories(${target_name}
		PUBLIC
			"${path_to_target}/inc/"
		PRIVATE
			"${path_to_target}/src/"
	)

	message("Target Added: ${target_name} (Dynamic Library)")

endfunction()


function(add_custom_test test_name target_name)


	set(path_to_target "${CMAKE_SOURCE_DIR}/projects/${target_name}")
	set(private_sources "")

	if("${language}" STREQUAL "CPP")
	file(GLOB_RECURSE private_sources 
		"${path_to_target}/inc/${target_name}/*.hpp"
		"${path_to_target}/src/${target_name}/*.hpp"
		"${path_to_target}/src/${target_name}/*.cpp"
		"${path_to_target}/tests/${test_name}.cpp"
		"${path_to_target}/inc/${target_name}/*.h"
		"${path_to_target}/src/${target_name}/*.h"
		"${path_to_target}/src/${target_name}/*.c"
		"${path_to_target}/tests/${test_name}.c"
	)
	elseif("${language}" STREQUAL "C")
	file(GLOB_RECURSE private_sources 
		"${path_to_target}/inc/${target_name}/*.h"
		"${path_to_target}/src/${target_name}/*.h"
		"${path_to_target}/src/${target_name}/*.c"
		"${path_to_target}/tests/${test_name}.c"
	)
	endif()

	add_executable(${test_name} ${private_sources})

	target_common(${test_name})

	target_include_directories(${test_name}
		PRIVATE
			"${path_to_target}/inc/"	
			"${path_to_target}/src/"	
			"${path_to_target}/tests/"	
	)

	get_target_property(defines 
		${target_name} INTERFACE_COMPILE_DEFINITIONS
	)
	if(defines)
    	target_compile_definitions(${test_name}
			PUBLIC 
				${defines}
		)
	endif()

	set_target_properties(${test_name} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${test_directory}"
	)

	add_test(NAME ${test_name}
		COMMAND ${test_name}
	)

	message("Test Added for ${target_name}: ${test_name}")
endfunction()

