
option(testing_build "make tests" OFF)
if(testing_build)
	enable_testing()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)



set(output_directory "${CMAKE_SOURCE_DIR}/_outputs")
set(exe_directory "${output_directory}/${CMAKE_BUILD_TYPE}")
set(test_directory "${output_directory}/${CMAKE_BUILD_TYPE}/_test")
set(junk_directory "${output_directory}/${CMAKE_BUILD_TYPE}/_junk")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${exe_directory}) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${exe_directory}) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${junk_directory}) 

#set(build_configs Debug Release RelWithDebInfo MinSizeRel)
#foreach(build_config IN LISTS build_configs)
#	string(TOUPPER ${build_config} config_u)
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config_u} ${output_directory}/${build_config})
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config_u} ${output_directory}/${build_config})
#	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config_u} ${output_directory}/${build_config}/_junk)
#endforeach()


