if(DEFINED passed_tests)
	enable_testing()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(language "")
if(CMAKE_CXX_COMPILER_LOADED)
	set(language "CPP")
elseif(CMAKE_C_COMPILER_LOADED)
	set(language "C")
else()
	message(FATAL_ERROR "Bad Compiler Setting")
endif()



set(output_directory "${CMAKE_SOURCE_DIR}/_outputs")
set(exe_directory "${output_directory}/${CMAKE_BUILD_TYPE}")
set(test_directory "${output_directory}/${CMAKE_BUILD_TYPE}/_test")
set(junk_directory "${output_directory}/${CMAKE_BUILD_TYPE}/_junk")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${exe_directory}) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${exe_directory}) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${junk_directory}) 


# Global exception-free setup 
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-Werror=exceptions) 
 		add_compile_options(-fno-exceptions)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-Werror=exceptions)
    add_compile_options(-fno-exceptions)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/EHs-c-)
    add_compile_definitions(_HAS_EXCEPTIONS=0)
endif()

#set(build_configs Debug Release RelWithDebInfo MinSizeRel)
#foreach(build_config IN LISTS build_configs)
#	string(TOUPPER ${build_config} config_u)
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config_u} ${output_directory}/${build_config})
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config_u} ${output_directory}/${build_config})
#	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config_u} ${output_directory}/${build_config}/_junk)
#endforeach()


